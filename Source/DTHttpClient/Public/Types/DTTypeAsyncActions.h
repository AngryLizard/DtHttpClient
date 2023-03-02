
#pragma once

#include "CoreMinimal.h"
#include "Http/DTHttpRequester.h"
#include "Types/DTTypeSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DTTypeAsyncActions.generated.h"


UCLASS(MinimalAPI)
class UDTFetchTypesAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFetchTypesDelegate, const FTypeSetup&, TypeSetup);

	UPROPERTY(BlueprintAssignable)
		FOnFetchTypesDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnFetchTypesDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTFetchTypesAsyncAction* FetchTypes(UObject* InWorldContextObject);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

private:
	UPROPERTY()
		UObject* WorldContextObject;
};

