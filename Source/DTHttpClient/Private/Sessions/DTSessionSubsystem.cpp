
#include "Sessions/DTSessionSubsystem.h"
#include "Generated/DTModels.h"
#include "Generated/DTObjects.h"
#include "Generated/DTApi.h"
#include "DTHttpClient.h"
#include "Algo/Transform.h"

#include "DTHttpClientSettings.h"
#include "Structures/CallPromise.h"

UDTSessionSubsystem* UDTSessionSubsystem::Get(UGameInstance* GameInstance)
{
	check(GameInstance);
	return GameInstance->GetSubsystem<UDTSessionSubsystem>();
}

bool UDTSessionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance();
}


void UDTSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();
	if (Settings->bAutoCreateSession)
	{
		UDTHttpClientSubsystem* ClientSubsystem = Collection.InitializeDependency<UDTHttpClientSubsystem>();
		check(ClientSubsystem);
		
		ClientSubsystem->OnLoggedIn.AddUObject(this, &UDTSessionSubsystem::OnAutoStartSession);
	}
}

void UDTSessionSubsystem::Deinitialize()
{
	if (ActiveSSEContext.IsSet())
	{
		if (ActiveSSEContext.GetValue())
		{
			ActiveSSEContext->Get()->Stop();
		}
		ActiveSSEContext.Reset();
	}

#if WITH_EDITOR
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	ClientSubsystem->OnLoggedIn.RemoveAll(this);
#endif

	Super::Deinitialize();
}

TWebPromisePtr<void> UDTSessionSubsystem::StartSession(const TArray<FDTEntityRef>& EntityRefs)
{
	return ActiveCallQueue.Await()
		->WeakThen(this, [this, EntityRefs]()
		{
			UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
			check(ClientSubsystem);

			FDTObserveData Data;
			Data.EntityRefs = ToStrings(EntityRefs);
			return UDTApi::PostSessionStart(ClientSubsystem->GetContext(), Data);
		})
		->WeakThen(this, [this, EntityRefs](const FResponseData<FString>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Starting session failed");
				return TWebPromise<void>::Rejected(FWebError());
			}
			
			ActiveSession = Response.Body;
			ObservedEntities = EntityRefs;
			RestartUpdateTimer();

			OnSessionStart.Broadcast(Response.Body, EntityRefs);
			OnSessionStart_Blueprint.Broadcast(Response.Body, EntityRefs);
			return TWebPromise<void>::Accepted();
		});
		/* TODO: Implement server side events properly :)
		->WeakThen(this, [this]()
		{
			UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
			check(ClientSubsystem);

			ActiveSSEContext = UDTHttpRequester::StartSSESession(ClientSubsystem->GetContext(), TEXT("/session/events") / SanitizeArg(ActiveSession.GetValue()),
				UDTHttpRequester::FSSEResultCallback<TArray<FDTPropertyChange>>::CreateWeakLambda(this, [this](const TArray<FDTPropertyChange>& Result)
				{
					UE_LOG(DTHttpClient, Log, TEXT("Got changes from SSE> %d"), Result.Num());
				}));

			return TWebPromise<void>::Accepted();
		});
		*/
}

TWebPromisePtr<void> UDTSessionSubsystem::StopSession()
{
	if (!ActiveSession.IsSet())
	{
		return TWebPromise<void>::Rejected(FWebError());
	}

	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	// Wait on any active session updates
	return ActiveCallQueue.Await()
		->WeakThen(this, [this]()
		{
			UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
			check(ClientSubsystem);

			return UDTApi::DeleteSessionStopWith(ClientSubsystem->GetContext(), ActiveSession.GetValue(), FEmptyBody());
		})
		->WeakThen(this, [this](const FResponseData<FEmptyResult>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Starting session failed");
				return TWebPromise<void>::Rejected(FWebError());
			}

			StopUpdateTimer();
			ActiveSession.Reset();
			ObservedEntities.Reset();
			if (ActiveSSEContext.IsSet())
			{
				if (ActiveSSEContext.GetValue())
				{
					ActiveSSEContext->Get()->Stop();
				}
				ActiveSSEContext.Reset();
			}
			return TWebPromise<void>::Accepted();
		});
}

bool UDTSessionSubsystem::HasActiveSession() const
{
	return ActiveSession.IsSet();
}

FString UDTSessionSubsystem::GetActiveSession() const
{
	check(ActiveSession.IsSet())
	return ActiveSession.GetValue();
}

TArray<FDTEntityRef> UDTSessionSubsystem::GetGameObjectRefs() const
{
	TArray<FDTEntityRef> GameObjectRefs;
	GameObjects.GetKeys(GameObjectRefs);
	return GameObjectRefs;
}

UDTGameBaseObject* UDTSessionSubsystem::GetGameObject(const FDTEntityRef& EntityRef) const
{
	if (UDTGameBaseObject* const* Ptr = GameObjects.Find(EntityRef))
	{
		return *Ptr;
	}
	return nullptr;
}

void UDTSessionSubsystem::RestartUpdateTimer()
{
	bHasActiveUpdateTimer = true;
	LastUpdateTime = FDateTime::UtcNow();

	// Clear timer in case one is already running
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
	OnTimerUpdate(true);
}

void UDTSessionSubsystem::StopUpdateTimer()
{
	bHasActiveUpdateTimer = false;
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
}

void UDTSessionSubsystem::OnTimerUpdate(bool Refresh)
{
	ActiveCallQueue.Queue(
		FWebPromiseQueue::FQueueDelegate::CreateWeakLambda(this, [this, Refresh]()
		{
			return UpdateSession(LastUpdateTime, Refresh)
				->WeakThen(this, [this](const FDateTime& Time)
				{
					LastUpdateTime = Time;
					const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();

					GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, FTimerDelegate::CreateUObject(this, &UDTSessionSubsystem::OnTimerUpdate, false), Settings->UpdateFrequency, false);
				});
		}));
}

TWebPromisePtr<FDateTime> UDTSessionSubsystem::UpdateSession(const FDateTime& Time, bool Refresh)
{
	if (!ActiveSession.IsSet())
	{
		return TWebPromise<FDateTime>::Rejected(FWebError());
	}

	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	const EDTPropertyQuery Query = Refresh ? EDTPropertyQuery::Refresh : EDTPropertyQuery::Diff;
	return UDTApi::PostSessionUpdateWith(ClientSubsystem->GetContext(), ActiveSession.GetValue(), Time, Query, FEmptyBody())
		->WeakThen(this, [this](const FResponseData<FDTSessionUpdate>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Updating session failed");
				return TWebPromise<FDateTime>::Rejected(FWebError());
			}

			if (!bHasActiveUpdateTimer)
			{
				UE_LOG(DTHttpClient, Warning, TEXT("Got an update when update time was already off, ignoring..."));
				return TWebPromise<FDateTime>::Rejected(FWebError());
			}

			UE_LOG(DTHttpClient, Log, TEXT("Received session update with %d changes"), Response.Body.Changes.Num());

			PushChanges(Response.Body.Changes, true);
			return TWebPromise<FDateTime>::Accepted(Response.Body.Timestamp);
		});
}


void UDTSessionSubsystem::PushChanges(const TArray<FDTPropertyChange>& Changes, bool bRetry)
{
	TSet<FDTEntityRef> MissingEntityRefs;
	TArray<FDTPropertyChange> PendingChanges;

	for (const FDTPropertyChange& Change : Changes)
	{
		const FDTEntityRef EntityRef = ToEntityRef(Change.EntityRef);

		// If we don't know about this entity yet update later
		if (!GameObjects.Contains(EntityRef))
		{
			MissingEntityRefs.Emplace(EntityRef);
			PendingChanges.Emplace(Change);
			continue;
		}

		GameObjects[EntityRef]->PushChange(Change);
	}

	// No need to update if all entities were known
	if (MissingEntityRefs.Num() == 0)
	{
		return;
	}

	if (!bRetry)
	{
		UE_LOG(DTHttpClient, Error, TEXT("Couldn't apply all changes because not all GameObjects exist"));
		return;
	}

	// Clear timer in case one is already running
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);

	UE_LOG(DTHttpClient, Log, TEXT("Not all changes were applied, waiting for object creation. Pending changes: %d"), PendingChanges.Num());

	ActiveCallQueue.Queue(
		FWebPromiseQueue::FQueueDelegate::CreateWeakLambda(this, [this, MissingEntityRefs, PendingChanges]()
		{
			return UpdateGameObjects(MissingEntityRefs.Array())
				->WeakThen(this, [this, PendingChanges]()
				{
					PushChanges(PendingChanges, false);
				});
		}));
}

TWebPromisePtr<void> UDTSessionSubsystem::UpdateGameObjects(const TArray<FDTEntityRef>& EntityRefs)
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	return UDTApi::GetGameListEntities(ClientSubsystem->GetContext(), ToStrings(EntityRefs), FEmptyBody())
		->WeakThen(this, [this](const FResponseData<TArray<FDTEntityData>>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Updating game objects failed");
				return TWebPromise<void>::Rejected(FWebError());
			}

			// Update or create received game objects
			for (const FDTEntityData& EntityData : Response.Body)
			{
				const FDTEntityRef EntityRef = ToEntityRef(EntityData.Ref);

				// Create if not exists
				if (!GameObjects.Contains(EntityRef))
				{
					EDTType Type;
					LexFromString(Type, *EntityData.TypeReg);
					TSubclassOf<UDTGameBaseObject> GameObjectClass = GetTypeObjectClass(Type);
					if (!*GameObjectClass)
					{
						UE_LOG(DTHttpClient, Error, TEXT("Couldn't create GameObject, class not found for type %s"), *EntityData.TypeReg);
						continue;
					}

					UDTGameBaseObject* GameObject = NewObject<UDTGameBaseObject>(this, GameObjectClass);
					GameObjects.Emplace(EntityRef, GameObject);
					UE_LOG(DTHttpClient, Log, TEXT("Created new game object: %s"), *EntityData.Name);
				}

				// Update values
				UDTGameBaseObject* GameObject = GameObjects[EntityRef];
				GameObject->Name = EntityData.Name;
				GameObject->TypeReg = EntityData.TypeReg;
				GameObject->OwnerRefs = EntityData.OwnerRefs;

				for (const FDTAttributeData& Attr : EntityData.Attributes)
				{
					GameObject->PushAttributes(Attr);
				}

				OnGameObjectUpdate.Broadcast(EntityRef, GameObject);
			}

			return TWebPromise<void>::Accepted();
		});
}

void UDTSessionSubsystem::OnAutoStartSession(const FUserInfo& UserInfo)
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(GetGameInstance());
	check(ClientSubsystem);

	UDTApi::GetGameEntityListWith(ClientSubsystem->GetContext(), LexToString(EDTType::Character), FEmptyBody())
		->WeakThen(this, [this](const FResponseData<TArray<FDTEntityData>>& Response)
		{
			TArray<FDTEntityRef> EntityRefs;
			Algo::Transform(Response.Body, EntityRefs, [](const FDTEntityData& Entity)
			{
				return ToEntityRef(Entity.Ref);
			});

			UE_LOG(DTHttpClient, Warning, TEXT("Player logged in, automatically starting session..."));
			StartSession(EntityRefs);
		});
}