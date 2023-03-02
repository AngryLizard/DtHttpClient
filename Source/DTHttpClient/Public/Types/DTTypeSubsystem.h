
#pragma once

#include "CoreMinimal.h"
#include "Types/DTTypeInfo.h"
#include "Generated/DTAssets.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "DTTypeSubsystem.generated.h"

class UDTGameBaseObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTypeSetupReceivedDelegate_Blueprint, const FTypeSetup&, Setup);

UCLASS()
class DTHTTPCLIENT_API UDTTypeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDTTypeSubsystem* Get(UGameInstance* GameInstance);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	* Get factory asset for a given type
	*/
	bool GetTypeAsset(const FString& TypeReg, FDTFactoryAsset& FactoryAsset) const;

	/**
	* Update with available types from server
	*/
	TWebPromisePtr<FTypeSetup> FetchTypes();

	UPROPERTY(BlueprintAssignable, DisplayName = "On Type Setup Received")
		FOnTypeSetupReceivedDelegate_Blueprint OnTypesReceived_Blueprint;

	using FOnTypeSetupReceivedDelegate = TMulticastDelegate<void(const FTypeSetup&)>;
	FOnTypeSetupReceivedDelegate OnTypesReceived;

private:

	UPROPERTY()
		FTypeSetup GameTypes;
};