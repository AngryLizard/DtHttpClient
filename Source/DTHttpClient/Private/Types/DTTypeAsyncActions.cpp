
#include "Types/DTTypeAsyncActions.h"
#include "Types/DTTypeSubsystem.h"

UDTFetchTypesAsyncAction* UDTFetchTypesAsyncAction::FetchTypes(UObject* InWorldContextObject)
{
	UDTFetchTypesAsyncAction* Action = NewObject<UDTFetchTypesAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	return Action;
}

void UDTFetchTypesAsyncAction::Activate()
{
	UDTTypeSubsystem* TypeSubsystem = UDTTypeSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(TypeSubsystem);

	TypeSubsystem->FetchTypes()
		->WeakThen(this, [this](const FTypeSetup& Actions)
		{
			OnSuccess.Broadcast(Actions);
		})
			->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast(FTypeSetup());
		});
}