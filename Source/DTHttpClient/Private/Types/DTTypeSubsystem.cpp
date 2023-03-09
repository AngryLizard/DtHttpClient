
#include "Types/DTTypeSubsystem.h"
#include "DTHttpClientSubsystem.h"
#include "Generated/DTApi.h"

#include "DTHttpClientSettings.h"
#include "Structures/CallPromise.h"

UDTTypeSubsystem* UDTTypeSubsystem::Get(UGameInstance* GameInstance)
{
	check(GameInstance);
	return GameInstance->GetSubsystem<UDTTypeSubsystem>();
}

bool UDTTypeSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance();
}

void UDTTypeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();
	UDTHttpClientSubsystem* ClientSubsystem = Collection.InitializeDependency<UDTHttpClientSubsystem>();
	check(ClientSubsystem);

	if (Settings->bAutoFetchTypes)
	{
		ClientSubsystem->OnLoggedIn.AddWeakLambda(this, [this](const FUserInfo&) { FetchTypes(); });
	}
}

void UDTTypeSubsystem::Deinitialize()
{
#if WITH_EDITOR
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	ClientSubsystem->OnLoggedIn.RemoveAll(this);
#endif

	Super::Deinitialize();
}

bool UDTTypeSubsystem::GetTypeAsset(const FString& TypeReg, FDTFactoryAsset& FactoryAsset) const
{
	// Recursively look for asset
	const FTypeInfo* TypeRegPtr = GameTypes.Infos.Find(TypeReg);
	if (!TypeRegPtr)
	{
		return false;
	}

	if (TypeRegPtr->Asset.IsEmpty())
	{
		return GetTypeAsset(TypeRegPtr->Parent, FactoryAsset);
	}

	LexFromString(FactoryAsset, *TypeRegPtr->Asset);
	return true;
}

TWebPromisePtr<FTypeSetup> UDTTypeSubsystem::FetchTypes()
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return UDTApi::GetGameTypes(ClientSubsystem->GetContext(), FEmptyBody())
		->WeakThen(this, [](const FResponseData<TArray<FDTTypeData>>& Response)
	{
		if (!Response.IsOk())
		{
			Response.Report("Getting types failed");
			return TWebPromise<FTypeSetup>::Rejected(FWebError());
		}

	FTypeSetup Setup;
	for (const FDTTypeData& Type : Response.Body)
	{
		FTypeInfo TypeInfo;
		TypeInfo.Asset = Type.Asset;
		TypeInfo.Parent = Type.Parent;
		TypeInfo.Description = Type.Desc;
		for (const FDTTypePropData& Prop : Type.Props)
		{
			FTypePropInfo PropInfo;
			PropInfo.Description = Prop.Desc;
			PropInfo.Type = Prop.Type;
			PropInfo.Initial = Prop.Initial;
			TypeInfo.Props.Emplace(Prop.PropReg, PropInfo);
		}
		for (const FDTTypePropData& Attr : Type.Attrs)
		{
			FTypePropInfo AttrInfo;
			AttrInfo.Description = Attr.Desc;
			AttrInfo.Type = Attr.Type;
			AttrInfo.Initial = Attr.Initial;
			TypeInfo.Attrs.Emplace(Attr.PropReg, AttrInfo);
		}
		Setup.Infos.Emplace(Type.TypeReg, TypeInfo);
	}

	return TWebPromise<FTypeSetup>::Accepted(Setup);
	})
		->WeakThen(this, [this](const FTypeSetup& Setup)
	{
		GameTypes = Setup;

	OnTypesReceived_Blueprint.Broadcast(Setup);
	OnTypesReceived.Broadcast(Setup);

	return Setup;
	});
}
