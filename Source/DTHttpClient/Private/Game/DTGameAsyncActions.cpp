
#include "Game/DTGameAsyncActions.h"

UDTGameGetEntitiesAsyncAction* UDTGameGetEntitiesAsyncAction::GetEntities(UObject* InWorldContextObject, EDTType Type)
{
	UDTGameGetEntitiesAsyncAction* Action = NewObject<UDTGameGetEntitiesAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	Action->Type = Type;
	return Action;
}

void UDTGameGetEntitiesAsyncAction::Activate()
{
	UDTGameSubsystem* GameSubsystem = UDTGameSubsystem::Get(WorldContextObject->GetWorld());
	check(GameSubsystem);

	GameSubsystem->GetEntitiesOfType(Type)
		->WeakThen(this, [this](const TArray<FDTEntityInfo>& Entities)
		{
			OnSuccess.Broadcast(Entities);
		})
		->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast(TArray<FDTEntityInfo>());
		});
}

