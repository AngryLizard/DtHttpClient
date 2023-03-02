
#include "Game/DTGameFactories.h"
#include "Game/DTGameBaseObject.h"
#include "Generated/DTObjects.h"
#include "GameFramework/GameStateBase.h"
#include "DTHttpClient.h"

UDTGameBaseComponent* UDTGameFactoryAsset::Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	return nullptr;
}

void UDTGameFactoryAsset::Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	if (IsValid(GameComponent))
	{
		GameComponent->EntityRef = EntityRef;
		if (!GameComponent->TryAssignObject(GameObject))
		{
			UE_LOG(DTHttpClient, Error, TEXT("Failed to update game object %s"), *EntityRef.Value);
			return;
		}
		GameComponent->GameUpdate();
	}
}

UDTGameBaseComponent* UDTWorldFactoryAsset::Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	Super::Build(World, EntityRef, GameObject);

	UDTAssetSubsystem* AssetSubsystem = UDTAssetSubsystem::Get(World->GetGameInstance());
	check(AssetSubsystem);

	UDTGameWorldObject* WorldObject = Cast<UDTGameWorldObject>(GameObject);
	if (IsValid(WorldObject))
	{
		TSoftObjectPtr<UWorld>* LevelAssetPtr = AssetSubsystem->LevelAssets.Find(WorldObject->GetLevel());
		if (LevelAssetPtr)
		{
			// TODO: Handle world spawning

			AGameStateBase* GameState = World->GetGameState();
			if (IsValid(GameState))
			{
				return GameState->FindComponentByClass<UDTGameBaseComponent>();
			}
		}
	}
	return nullptr;
}

void UDTWorldFactoryAsset::Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	Super::Update(GameComponent, EntityRef, GameObject);
}

UDTGameBaseComponent* UDTActorFactoryAsset::Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	Super::Build(World, EntityRef, GameObject);

	UDTAssetSubsystem* AssetSubsystem = UDTAssetSubsystem::Get(World->GetGameInstance());
	check(AssetSubsystem);

	UDTGameActorObject* ActorObject = Cast<UDTGameActorObject>(GameObject);
	if (IsValid(ActorObject))
	{
		TSubclassOf<AActor>* ActorAssetPtr = AssetSubsystem->ActorAssets.Find(ActorObject->GetActor());
		if (ActorAssetPtr)
		{
			// TODO: Handle actor spawning

			AActor* Actor = World->SpawnActor(*ActorAssetPtr);
			if (IsValid(Actor))
			{
				UDTGameBaseComponent* GameComponent = Actor->FindComponentByClass<UDTGameBaseComponent>();
				if (IsValid(GameComponent))
				{
					return GameComponent;
				}
				World->DestroyActor(Actor);
			}
		}
	}
	return nullptr;
}

void UDTActorFactoryAsset::Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject)
{
	Super::Update(GameComponent, EntityRef, GameObject);
}
