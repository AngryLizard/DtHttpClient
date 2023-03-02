
#pragma once

#include "CoreMinimal.h"
#include "Http/DTHttpRequester.h"
#include "Actions/DTActionSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DTActionAsyncActions.generated.h"

UCLASS(MinimalAPI)
class UDTFetchActionsAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFetchActionsDelegate, const FActionSetup&, Setup);

	UPROPERTY(BlueprintAssignable)
		FOnFetchActionsDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnFetchActionsDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTFetchActionsAsyncAction* FetchActions(UObject* InWorldContextObject);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

private:
	UPROPERTY()
		UObject* WorldContextObject;
};


UCLASS(MinimalAPI)
class UDTRunActionAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRunActionDelegate);

	UPROPERTY(BlueprintAssignable)
		FOnRunActionDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnRunActionDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTRunActionAsyncAction* RunAction(UObject* InWorldContextObject, const FDTEntityRef& CallerRef, const FString& Name, const TMap<FString, FSerialPropertyValue>& Params);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

private:
	UPROPERTY()
		FDTEntityRef CallerRef;

	UPROPERTY()
		FString Name;

	UPROPERTY()
		TMap<FString, FSerialPropertyValue> Params;

	UPROPERTY()
		UObject* WorldContextObject;
};
