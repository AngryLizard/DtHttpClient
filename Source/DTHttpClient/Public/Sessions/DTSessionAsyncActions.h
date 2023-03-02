
#pragma once

#include "CoreMinimal.h"
#include "Http/DTHttpRequester.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DTSessionAsyncActions.generated.h"

UCLASS(MinimalAPI)
class UDTSessionAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPromiseDelegate);

	UPROPERTY(BlueprintAssignable)
		FOnPromiseDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnPromiseDelegate OnFailure;

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const;

	UPROPERTY()
		UObject* WorldContextObject;
};

UCLASS(MinimalAPI)
class UDTSessionStartAsyncAction : public UDTSessionAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTSessionStartAsyncAction* StartSession(UObject* InWorldContextObject, const TArray<FDTEntityRef>& EntityRefs);

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const override;

private:
	UPROPERTY()
		TArray<FDTEntityRef> EntityRefs;
};

UCLASS(MinimalAPI)
class UDTSessionStopAsyncAction : public UDTSessionAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTSessionStopAsyncAction* StopSession(UObject* InWorldContextObject);

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const override;

};
