
#include "Sessions/DTSessionAsyncActions.h"
#include "Sessions/DTSessionSubsystem.h"

void UDTSessionAsyncActionBase::Activate()
{
	ActivatePromise()
		->WeakThen(this, [this]()
		{
			OnSuccess.Broadcast();
		})
		->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast();
		});
}

TWebPromisePtr<void> UDTSessionAsyncActionBase::ActivatePromise() const
{
	return TWebPromise<void>::Rejected(FWebError());
}


UDTSessionStartAsyncAction* UDTSessionStartAsyncAction::StartSession(UObject* WorldContextObject, const TArray<FDTEntityRef>& EntityRefs)
{
	UDTSessionStartAsyncAction* Action = NewObject<UDTSessionStartAsyncAction>();
	Action->WorldContextObject = WorldContextObject;
	Action->EntityRefs = EntityRefs;
	return Action;
}

TWebPromisePtr<void> UDTSessionStartAsyncAction::ActivatePromise() const
{
	UDTSessionSubsystem* SessionSubsystem = UDTSessionSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(SessionSubsystem);

	return SessionSubsystem->StartSession(EntityRefs);
}


UDTSessionStopAsyncAction* UDTSessionStopAsyncAction::StopSession(UObject* WorldContextObject)
{
	UDTSessionStopAsyncAction* Action = NewObject<UDTSessionStopAsyncAction>();
	Action->WorldContextObject = WorldContextObject;
	return Action;
}

TWebPromisePtr<void> UDTSessionStopAsyncAction::ActivatePromise() const
{
	UDTSessionSubsystem* SessionSubsystem = UDTSessionSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(SessionSubsystem);

	return SessionSubsystem->StopSession();
}