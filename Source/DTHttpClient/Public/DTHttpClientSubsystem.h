
#pragma once

#include "CoreMinimal.h"

#include "Utils/DTIdentifiers.h"
#include "Http/DTHttpRequester.h"
#include "Http/DTHttpContext.h"
#include "Misc/Optional.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "DTHttpClientSubsystem.generated.h"

/**
* User role that determines permission.
* Has to numerically match the roles on server.
*/
UENUM(BlueprintType)
enum class EUserRole : uint8
{
	Spectator = 0,
	Participant = 1,
	Gamemaster = 2,
	Moderator = 3,
	Admin = 4,
};

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString Ref = "";

	UPROPERTY()
		FString Nickname = "";

	UPROPERTY()
		EUserRole Role = EUserRole::Spectator;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoggedInDelegate_Blueprint, const FUserInfo&, Info);

UCLASS()
class DTHTTPCLIENT_API UDTHttpClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDTHttpClientSubsystem* Get(UGameInstance* GameInstance);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	* Login with given user
	*/
	TWebPromisePtr<void> Login(const FString& Username, const FString& Password);

	/**
	* Register user on server
	*/
	TWebPromisePtr<void> Register(const FString& Nickname, const FString& Username, const FString& Password);

	/**
	* Logout with current user
	*/
	TWebPromisePtr<void> Logout();

	UFUNCTION(BlueprintPure)
		bool IsLoggedIn() const;

	UFUNCTION(BlueprintPure)
		const FUserInfo& GetUserInfo() const;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Logged In")
		FOnLoggedInDelegate_Blueprint OnLoggedIn_Blueprint;

	using FOnLoggedInDelegate = TMulticastDelegate<void(const FUserInfo&)>;
	FOnLoggedInDelegate OnLoggedIn;

	const FDTHttpContext& GetContext() const;

private:
	UPROPERTY()
		FDTHttpContext Context;

	TOptional<FUserInfo> UserInfo;
};