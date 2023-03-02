
#include "DTHttpClientAsyncActions.h"
#include "DTHttpClientSubsystem.h"

void UDTHttpClientAsyncActionBase::Activate()
{
	ActivatePromise()
		->WeakThen(this, [this]()
		{
			OnSuccess.Broadcast();
		})
		->WeakCatch(this, [this](const FWebError& Error)
		{
			OnFailure.Broadcast();
		});
}

TWebPromisePtr<void> UDTHttpClientAsyncActionBase::ActivatePromise() const
{
	return TWebPromise<void>::Rejected(FWebError());
}


UDTHttpClientLoginAsyncAction* UDTHttpClientLoginAsyncAction::Login(UObject* InWorldContextObject, const FString& Username, const FString& Password)
{
	UDTHttpClientLoginAsyncAction* Action = NewObject<UDTHttpClientLoginAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	Action->Username = Username;
	Action->Password = Password;
	return Action;
}

TWebPromisePtr<void> UDTHttpClientLoginAsyncAction::ActivatePromise() const
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return ClientSubsystem->Login(Username, Password);
}


UDTHttpClientRegisterAsyncAction* UDTHttpClientRegisterAsyncAction::Register(UObject* InWorldContextObject, const FString& Nickname, const FString& Username, const FString& Password)
{
	UDTHttpClientRegisterAsyncAction* Action = NewObject<UDTHttpClientRegisterAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	Action->Nickname = Nickname;
	Action->Username = Username;
	Action->Password = Password;
	return Action;
}

TWebPromisePtr<void> UDTHttpClientRegisterAsyncAction::ActivatePromise() const
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return ClientSubsystem->Register(Nickname, Username, Password);
}


UDTHttpClientLogoutAsyncAction* UDTHttpClientLogoutAsyncAction::Logout(UObject* InWorldContextObject)
{
	UDTHttpClientLogoutAsyncAction* Action = NewObject<UDTHttpClientLogoutAsyncAction>();
	Action->WorldContextObject = InWorldContextObject;
	return Action;
}

TWebPromisePtr<void> UDTHttpClientLogoutAsyncAction::ActivatePromise() const
{
	UDTHttpClientSubsystem* ClientSubsystem = UDTHttpClientSubsystem::Get(WorldContextObject->GetWorld()->GetGameInstance());
	check(ClientSubsystem);

	return ClientSubsystem->Logout();
}
