
#pragma once

#include "CoreMinimal.h"

#include "Utils/DTIdentifiers.h"
#include "Http/DTHttpContext.h"
#include "Http/DTHttpRequester.h"
#include "DTHttpClientSubsystem.h"
#include "Misc/Optional.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "DTSessionSubsystem.generated.h"

class UDTGameBaseObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSessionStartDelegate_Blueprint, FString, SessionRef, const TArray<FDTEntityRef>&, ObservedEntityRefs);

UCLASS()
class DTHTTPCLIENT_API UDTSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDTSessionSubsystem* Get(UGameInstance* GameInstance);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	* Start session with a list of entities to observe.
	*/
	TWebPromisePtr<void> StartSession(const TArray<FDTEntityRef>& EntityRefs);

	/**
	* Stop session
	*/
	TWebPromisePtr<void> StopSession();

	UFUNCTION(BlueprintPure)
		bool HasActiveSession() const;

	UFUNCTION(BlueprintPure)
		FString GetActiveSession() const;

	UFUNCTION(BlueprintPure)
		TArray<FDTEntityRef> GetGameObjectRefs() const;

	UFUNCTION(BlueprintPure)
		UDTGameBaseObject* GetGameObject(const FDTEntityRef& EntityRef) const;

	UPROPERTY(BlueprintAssignable, DisplayName = "On Session Start")
		FOnSessionStartDelegate_Blueprint OnSessionStart_Blueprint;

	using FOnSessionStartDelegate = TMulticastDelegate<void(const FString&, const TArray<FDTEntityRef>&)>;
	FOnSessionStartDelegate OnSessionStart;

	using FOnGameObjectUpdateDelegate = TMulticastDelegate<void(const FDTEntityRef&, UDTGameBaseObject*)>;
	FOnGameObjectUpdateDelegate OnGameObjectUpdate;

private:
	TOptional<FString> ActiveSession;
	TArray<FDTEntityRef> ObservedEntities;
	UDTHttpRequester::FSSEContextPtr ActiveSSEContext;

	FDateTime LastUpdateTime;
	FTimerHandle UpdateTimerHandle;
	bool bHasActiveUpdateTimer = false;

	FWebPromiseQueue ActiveCallQueue;

	void RestartUpdateTimer();
	void StopUpdateTimer();

	UFUNCTION()
		void OnTimerUpdate(bool Refresh);

	UFUNCTION()
		void OnSSEUpdate(const FDTSessionUpdate& Result);

	UFUNCTION()
		void OnSSEStop();

	// Get timeline changes since last update
	TWebPromisePtr<FDateTime> UpdateSession(const FDateTime& Time, bool Refresh);

	// Manage game objects
	void PushChanges(const TArray<FDTPropertyChange>& Changes, bool bRetry);
	TWebPromisePtr<void> UpdateGameObjects(const TArray<FDTEntityRef>& EntityRefs);

	UPROPERTY()
		TArray<FDTPropertyChange> ChangeQueue;

	UPROPERTY()
		TMap<FDTEntityRef, UDTGameBaseObject*> GameObjects;

	void OnAutoStartSession(const FUserInfo& UserInfo);
};