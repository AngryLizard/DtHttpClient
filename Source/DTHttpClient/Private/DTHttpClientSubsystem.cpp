
#include "DTHttpClientSubsystem.h"
#include "Generated/DTModels.h"
#include "Generated/DTApi.h"
#include "DTHttpClient.h"

#include "DTHttpClientSettings.h"
#include "Structures/CallPromise.h"

namespace
{
	EUserRole ConvertRole(EDTRole Role)
	{
		return (EUserRole)Role;
	}

	EDTRole ConvertRole(EUserRole Role)
	{
		return (EDTRole)Role;
	}
}

UDTHttpClientSubsystem* UDTHttpClientSubsystem::Get(UGameInstance* GameInstance)
{
	check(GameInstance);
	return GameInstance->GetSubsystem<UDTHttpClientSubsystem>();
}

bool UDTHttpClientSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return !CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance();
}


void UDTHttpClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDTHttpClientSettings* Settings = GetDefault<UDTHttpClientSettings>();

	Context.BaseUrl = FString::Printf(TEXT("%s:%d"), *Settings->ServerAddress, Settings->ServerPort);

#if WITH_EDITOR
	if (!Settings->bConnectInEditorRemote)
	{
		Context.BaseUrl = FString::Printf(TEXT("localhost:%d"), Settings->ServerPort);
	}

	const int32 PlayerId = GPlayInEditorID + 1;
	if (Settings->PlayInEditorUsers.IsValidIndex(PlayerId))
	{
		const FTestUserCredentials& Credentials = Settings->PlayInEditorUsers[PlayerId];
		UE_LOG(DTHttpClient, Log, TEXT("Logging in player with username %s"), *Credentials.Username);

		// Try to login, if it fails try to register, then login again.
		Login(Credentials.Username, Credentials.Password)
			->WeakCatch(this, [this, Credentials](const FWebError& Error)
			{
				UE_LOG(DTHttpClient, Warning, TEXT("Login failed, registering player."));
				return Register(Credentials.Nickname, Credentials.Username, Credentials.Password);
			})
			->WeakThen(this, [this, Credentials]()
			{
				UE_LOG(DTHttpClient, Log, TEXT("Login after registration."));
				return Login(Credentials.Username, Credentials.Password);
			});
	}
#endif
}

void UDTHttpClientSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

TWebPromisePtr<void> UDTHttpClientSubsystem::Login(const FString& Username, const FString& Password)
{
	FDTPlayerCredentials Credentials;
	Credentials.Username = Username;
	Credentials.Password = Password;

	return UDTApi::PostPlayerLogin(Context, Credentials)
		->WeakThen(this, [this](FResponseData<FDTTokenResult> Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Player login failed");
				return TWebPromise<FResponseData<FDTUserData>>::Rejected(FWebError());
			}

			Context.Token = Response.Body.Token;
			UE_LOG(DTHttpClient, Log, TEXT("Player login succeeded with token %s"), *Context.Token);
			return UDTApi::GetPlayer(Context, FEmptyBody());
		})
		->WeakThen(this, [this](const FResponseData<FDTUserData>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Getting player information failed");
				return TWebPromise<void>::Rejected(FWebError());
			}

			FUserInfo Info;
			Info.Ref = Response.Body.Ref;
			Info.Nickname = Response.Body.Nickname;
			Info.Role = ConvertRole(Response.Body.Access);
			UserInfo = Info;

			OnLoggedIn.Broadcast(Info);
			OnLoggedIn_Blueprint.Broadcast(Info);
			return TWebPromise<void>::Accepted();
		});
}

TWebPromisePtr<void> UDTHttpClientSubsystem::Register(const FString& Nickname, const FString& Username, const FString& Password)
{
	FDTPlayerRequest Request;
	Request.Nickname = Nickname;
	Request.Username = Username;
	Request.Password = Password;

	return UDTApi::PostPlayerRegister(Context, Request)
		->WeakThen(this, [this, Request](const FResponseData<FString>& Response)
		{
			if (!Response.IsOk())
			{
				Response.Report("Starting session failed");
				return TWebPromise<void>::Rejected(FWebError());
			}
			
			return TWebPromise<void>::Accepted();
		});
}

TWebPromisePtr<void> UDTHttpClientSubsystem::Logout()
{
	if (!UserInfo.IsSet())
	{
		return TWebPromise<void>::Rejected(FWebError());
	}

	UserInfo.Reset();
	Context.Token = {};

	return TWebPromise<void>::Accepted();
}

bool UDTHttpClientSubsystem::IsLoggedIn() const
{
	return UserInfo.IsSet();
}

const FUserInfo& UDTHttpClientSubsystem::GetUserInfo() const
{
	check(UserInfo.IsSet())
	return UserInfo.GetValue();
}

const FDTHttpContext& UDTHttpClientSubsystem::GetContext() const
{
	return Context;
}
