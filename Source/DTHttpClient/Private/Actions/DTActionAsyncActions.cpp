
#include "Actions/DTActionAsyncActions.h"
#include "Actions/DTActionSubsystem.h"
#include "Sessions/DTSessionSubsystem.h"

UDTFetchActionsAsyncAction* UDTFetchActionsAsyncAction::FetchActions(UObject* InWorldContextObject)
{
	UDTFetchActionsAsyncAction* Action = NewObject<UDTFetchActionsAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	return Action;
}

void UDTFetchActionsAsyncAction::Activate()
{
	UDTActionSubsystem* ActionSubsystem = UDTActionSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(ActionSubsystem);

	ActionSubsystem->FetchActions()
		->WeakThen(this, [this](const FActionSetup& Actions)
		{
			OnSuccess.Broadcast(Actions);
		})
			->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast(FActionSetup());
		});
}


UDTRunActionAsyncAction* UDTRunActionAsyncAction::RunAction(UObject* InWorldContextObject, const FDTEntityRef& CallerRef, const FString& Name, const TMap<FString, FSerialPropertyValue>& Params)
{
	UDTRunActionAsyncAction* Action = NewObject<UDTRunActionAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	Action->CallerRef = CallerRef;
	Action->Name = Name;
	Action->Params = Params;
	return Action;
}

void UDTRunActionAsyncAction::Activate()
{
	UDTActionSubsystem* ActionSubsystem = UDTActionSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(ActionSubsystem);

	ActionSubsystem->RunAction(CallerRef, Name, Params)
		->WeakThen(this, [this]()
		{
			OnSuccess.Broadcast();
		})
			->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast();
		});
}
