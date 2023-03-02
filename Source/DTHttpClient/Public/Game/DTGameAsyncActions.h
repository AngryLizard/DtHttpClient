
#pragma once

#include "CoreMinimal.h"
#include "Generated/DTTypes.h"
#include "Structures/CallPromise.h"
#include "Game/DTGameSubsystem.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DTGameAsyncActions.generated.h"

UCLASS(MinimalAPI)
class UDTGameGetEntitiesAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetEntitiesDelegate, const TArray<FDTEntityInfo>&, Entities);

	UPROPERTY(BlueprintAssignable)
		FOnGetEntitiesDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnGetEntitiesDelegate OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTGameGetEntitiesAsyncAction* GetEntities(UObject* InWorldContextObject, EDTType Type);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	// End of UBlueprintAsyncActionBase interface

private:
	UPROPERTY()
		EDTType Type;

	UPROPERTY()
		UObject* WorldContextObject;
};
