
#include "Actions/DTActionSubsystem.h"
#include "DTHttpClientSubsystem.h"
#include "Generated/DTApi.h"

#include "DTHttpClientSettings.h"
#include "Structures/CallPromise.h"

UDTActionSubsystem* UDTActionSubsystem::Get(UGameInstance* GameInstance)
{
	check(GameInstance);
	return GameInstance->GetSubsystem<UDTActionSubsystem>();
}

bool UDTActionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance();
}

void UDTActionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();
	UDTHttpClientSubsystem* ClientSubsystem = Collection.InitializeDependency<UDTHttpClientSubsystem>();
	check(ClientSubsystem);

	if (Settings->bAutoFetchActions)
	{
		ClientSubsystem->OnLoggedIn.AddWeakLambda(this, [this](const FUserInfo&) { FetchActions(); });
	}
}

void UDTActionSubsystem::Deinitialize()
{
#if WITH_EDITOR
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	ClientSubsystem->OnLoggedIn.RemoveAll(this);
#endif

	Super::Deinitialize();
}

TWebPromisePtr<FActionSetup> UDTActionSubsystem::FetchActions()
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return UDTApi::GetGameActions(ClientSubsystem->GetContext(), FEmptyBody())
		->WeakThen(this, [](const FResponseData<TArray<FDTActionData>>& Response)
	{
		if (!Response.IsOk())
		{
			Response.Report("Getting actions failed");
			return TWebPromise<FActionSetup>::Rejected(FWebError());
		}

	FActionSetup Setup;
	for (const FDTActionData& Action : Response.Body)
	{
		FActionInfo ActionInfo;
		ActionInfo.Domain = Action.Domain;
		ActionInfo.Trigger = Action.Trigger;
		ActionInfo.Description = Action.Desc;
		for (const FDTActionParamData& Param : Action.Params)
		{
			FSerialPropertyValueInfo ParamInfo;
			ParamInfo.Input = Param.Input;
			ParamInfo.Description = Param.Desc;
			ParamInfo.Type = Param.Type;
			ParamInfo.Initial = Param.Initial;
			ActionInfo.Params.Emplace(Param.ParamReg, ParamInfo);
		}
		Setup.Infos.Emplace(Action.ActionReg, ActionInfo);
	}

	return TWebPromise<FActionSetup>::Accepted(Setup);
	})
		->WeakThen(this, [this](const FActionSetup& Setup)
	{
		GameActions = Setup;

	OnActionsReceived_Blueprint.Broadcast(Setup);
	OnActionsReceived.Broadcast(Setup);

	return Setup;
	});
}

TWebPromisePtr<void> UDTActionSubsystem::RunAction(const FDTEntityRef& CallerRef, const FString& Name, const TMap<FString, FSerialPropertyValue>& Params)
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	if (!GameActions.Infos.Contains(Name))
	{
		UE_LOG(DTHttpClient, Error, TEXT("Couldn't find action %s in cached action list. Maybe it wasn't fetched yet?"), *Name);
		return TWebPromise<void>::Rejected(FWebError());
	}

	const FActionInfo& ActionInfo = GameActions.Infos[Name];

	TSharedRef<FJsonObject> ActionParams = MakeShareable(new FJsonObject);
	for (const auto& Param : Params)
	{
		if (!ActionInfo.Params.Contains(Param.Key))
		{
			UE_LOG(DTHttpClient, Error, TEXT("Couldn't find action param %s in action %s"), *Param.Key, *Name);
			return TWebPromise<void>::Rejected(FWebError());
		}

		EDTPropertyType PropertyType;
		LexFromString(PropertyType, *ActionInfo.Params[Param.Key].Type);
		if (PropertyType != Param.Value.Type)
		{
			UE_LOG(DTHttpClient, Error, TEXT("Type of %s param %s doesn't match input"), *Name, *Param.Key);
			return TWebPromise<void>::Rejected(FWebError());
		}

		ActionParams->SetStringField(Param.Key, Param.Value.Value);
	}

	const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();
	const float Delay = Settings->bSSEEnabled ? 0.f : Settings->UpdatePollFrequency;
	const FString Path = FString::Printf(TEXT("/action/%s/%s?delay=%.3f"), *LexToString(CallerRef), *Name, (Delay + Settings->MaxDelay));

	return UDTHttpRequester::SendHttpRequest<FEmptyResult>(ClientSubsystem->GetContext(), TEXT("POST"), Path, ActionParams)
		->WeakThen(this, [](const FResponseData<FEmptyResult>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Calling action failed");
				return TWebPromise<void>::Rejected(FWebError());
			}
			return TWebPromise<void>::Accepted();
		});
}