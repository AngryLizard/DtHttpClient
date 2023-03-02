
#include "Game/DTGameSubsystem.h"
#include "Game/DTGameFactories.h"
#include "Types/DTTypeSubsystem.h"
#include "Sessions/DTSessionSubsystem.h"
#include "DTHttpClientSubsystem.h"
#include "DTHttpClientSettings.h"
#include "Generated/DTApi.h"
#include "Generated/DTTypes.h"
#include "Generated/DTAssets.h"
#include "Generated/DTObjects.h"
#include "DTHttpClient.h"
#include "Algo/Transform.h"

UDTGameSubsystem* UDTGameSubsystem::Get(UWorld* World)
{
	check(World);
	return World->GetSubsystem<UDTGameSubsystem>();
}

bool UDTGameSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
#if WITH_EDITOR
	return CastChecked<UWorld>(Outer)->IsPlayInEditor();
#endif // WITH_EDITOR
	return true;
}

void UDTGameSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// We requires types to be loaded before starting
	UDTTypeSubsystem* TypeSubsystem = UDTTypeSubsystem::Get(InWorld.GetGameInstance());
	check(TypeSubsystem);

	TypeSubsystem->OnTypesReceived.AddUObject(this, &UDTGameSubsystem::OnTypeSetupReceived);
}

void UDTGameSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Only tick objects that are experiencing changes
	const FDateTime Timestamp = FDateTime::UtcNow();
	for (const auto& GameComponent : GameComponents)
	{
		if (GameComponent.Value.IsValid())
		{
			GameComponent.Value->GameTick(Timestamp);
		}
	}
}

TStatId UDTGameSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDTGameSubsystem, STATGROUP_Tickables);
}

TWebPromisePtr<TArray<FDTEntityInfo>> UDTGameSubsystem::GetEntitiesOfType(EDTType Type)
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return UDTApi::GetGameEntityListWith(ClientSubsystem->GetContext(), LexToString(Type), FEmptyBody())
		->WeakThen(this, [](const FResponseData<TArray<FDTEntityData>>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Getting entities failed");
				return TWebPromise<TArray<FDTEntityInfo>>::Rejected(FWebError());
			}

			TArray<FDTEntityInfo> Infos;
			Algo::Transform(Response.Body, Infos,
				[](const FDTEntityData& Entity)
				{
					FDTEntityInfo Info;
					Info.Ref = ToEntityRef(Entity.Ref);
					Info.Name = Entity.Name;
					LexFromString(Info.Type, *Entity.TypeReg);
					Info.OwnerRefs = Entity.OwnerRefs;
					for (const FDTAttributeData& Attr : Entity.Attributes)
					{
						FDTAttributeInfo AttrInfo;
						AttrInfo.PropReg = Attr.PropReg;
						AttrInfo.PropVal = Attr.PropVal;
						Info.Attributes.Emplace(AttrInfo);
					}
					
					return Info;
				});
			return TWebPromise<TArray<FDTEntityInfo>>::Accepted(Infos);
		});
}


void UDTGameSubsystem::OnGameObjectUpdate(const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	check(GameObject);

	UDTTypeSubsystem* TypeSubsystem = UDTTypeSubsystem::Get(GetWorld()->GetGameInstance());
	check(TypeSubsystem);

	FDTFactoryAsset FactoryAsset;
	if (!TypeSubsystem->GetTypeAsset(GameObject->TypeReg, FactoryAsset))
	{
		UE_LOG(DTHttpClient, Error, TEXT("Asset for Type %s not found for entity %s. Maybe types weren't fetched yet?"), *GameObject->TypeReg, *GameObject->Name);
		return;
	}

	UDTAssetSubsystem* AssetSubsystem = UDTAssetSubsystem::Get(GetWorld()->GetGameInstance());
	check(AssetSubsystem);

	UDTGameFactoryAsset** FactoryAssetPtr = AssetSubsystem->FactoryAssets.Find(FactoryAsset);
	if (!FactoryAssetPtr)
	{
		UE_LOG(DTHttpClient, Warning, TEXT("No factory found for asset %s"), *LexToString(FactoryAsset));
		return;
	}

	TWeakObjectPtr<UDTGameBaseComponent>& GameComponent = GameComponents.FindOrAdd(EntityRef, nullptr);

	if (!GameComponent.IsValid())
	{
		GameComponent = (*FactoryAssetPtr)->Build(GetWorld(), EntityRef, GameObject);
		if (!GameComponent.IsValid())
		{
			UE_LOG(DTHttpClient, Warning, TEXT("aborted build for %s"), *LexToString(EntityRef));
			return;
		}
	}

	(*FactoryAssetPtr)->Update(GameComponent.Get(), EntityRef, GameObject);
}

void UDTGameSubsystem::OnTypeSetupReceived(const FTypeSetup& Setup)
{
	UDTSessionSubsystem* SessionSubsystem = UDTSessionSubsystem::Get(GetWorld()->GetGameInstance());
	check(SessionSubsystem);

	// Get future game object updates
	SessionSubsystem->OnGameObjectUpdate.AddUObject(this, &UDTGameSubsystem::OnGameObjectUpdate);

	// Get all already existing game objects
	const TArray<FDTEntityRef> ObjectsRefs = SessionSubsystem->GetGameObjectRefs();
	for (const FDTEntityRef& ObjectsRef : ObjectsRefs)
	{
		UDTGameBaseObject* GameObject = SessionSubsystem->GetGameObject(ObjectsRef);
		OnGameObjectUpdate(ObjectsRef, GameObject);
	}
}
