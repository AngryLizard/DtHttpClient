
#pragma once

#include "CoreMinimal.h"
#include "Http/DTHttpRequester.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DTHttpClientAsyncActions.generated.h"

UCLASS(MinimalAPI)
class UDTHttpClientAsyncActionBase : public UBlueprintAsyncActionBase
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
class UDTHttpClientLoginAsyncAction : public UDTHttpClientAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTHttpClientLoginAsyncAction* Login(UObject* InWorldContextObject, const FString& Username, const FString& Password);

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const override;

private:
	FString Username;
	FString Password;
};

UCLASS(MinimalAPI)
class UDTHttpClientRegisterAsyncAction : public UDTHttpClientAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTHttpClientRegisterAsyncAction* Register(UObject* InWorldContextObject, const FString& Nickname, const FString& Username, const FString& Password);

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const override;

private:
	FString Nickname;
	FString Username;
	FString Password;
};

UCLASS(MinimalAPI)
class UDTHttpClientLogoutAsyncAction : public UDTHttpClientAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "DTHttp")
		static UDTHttpClientLogoutAsyncAction* Logout(UObject* InWorldContextObject);

protected:
	virtual TWebPromisePtr<void> ActivatePromise() const override;
};
