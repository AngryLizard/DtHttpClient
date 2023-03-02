
#pragma once

#include "CoreMinimal.h"
#include "Actions/DTActionInfo.h"
#include "Generated/DTAssets.h"

#include "Game/DTGameProperties.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DTActionSubsystem.generated.h"

class UDTGameBaseObject;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSetupReceivedDelegate_Blueprint, const FActionSetup&, Setup);

UCLASS()
class DTHTTPCLIENT_API UDTActionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDTActionSubsystem* Get(UGameInstance* GameInstance);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	/**
	* Update with available actions from server
	*/
	TWebPromisePtr<FActionSetup> FetchActions();

	/**
	* Sends an action call to the server
	*/
	TWebPromisePtr<void> RunAction(const FDTEntityRef& CallerRef, const FString& Name, const TMap<FString, FSerialPropertyValue>& Params);

	UPROPERTY(BlueprintAssignable, DisplayName = "On Action Setup Received")
		FOnActionSetupReceivedDelegate_Blueprint OnActionsReceived_Blueprint;

	using FOnActionSetupReceivedDelegate = TMulticastDelegate<void(const FActionSetup&)>;
	FOnActionSetupReceivedDelegate OnActionsReceived;

private:

	UPROPERTY()
		FActionSetup GameActions;
};