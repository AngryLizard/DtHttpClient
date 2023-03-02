
#pragma once

#include "CoreMinimal.h"
#include "DTGameBaseObject.h"
#include "Game/DTEntityInfo.h"
#include "Types/DTTypeSubsystem.h"
#include "Http/DTHttpRequester.h"

#include "Subsystems/WorldSubsystem.h"
#include "DTGameSubsystem.generated.h"

class UDTGameBaseComponent;

UCLASS()
class DTHTTPCLIENT_API UDTGameSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UDTGameSubsystem* Get(UWorld* World);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;


	/**
	* Get all accessible entities of a given type
	*/
	TWebPromisePtr<TArray<FDTEntityInfo>> GetEntitiesOfType(EDTType Type);

private:
	void OnGameObjectUpdate(const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject);

	void OnTypeSetupReceived(const FTypeSetup& Setup);

	UPROPERTY()
		TMap<FDTEntityRef, TWeakObjectPtr<UDTGameBaseComponent>> GameComponents;
};
