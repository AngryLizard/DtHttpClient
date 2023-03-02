
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DTHttpClientSettings.generated.h"

#if WITH_EDITORONLY_DATA
USTRUCT()
struct FTestUserCredentials
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString Nickname;

	UPROPERTY(EditAnywhere)
		FString Username;

	UPROPERTY(EditAnywhere)
		FString Password;
};
#endif //WITH_EDITORONLY_DATA

UCLASS(config = HttpClientSettings)
class DTHTTPCLIENT_API UDTHttpClientSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	/**
	* Automatically tries to login/register these users according to their client ID.
	* This works only for PIE builds and speeds up testing.
	*/
	UPROPERTY(config, EditAnywhere)
		TArray<FTestUserCredentials> PlayInEditorUsers;

	/**
	* Usually while testing we connect to localhost.
	* This switch turns this behaviour off and connects the PIE session to the remote server given by ServerAddress.
	*/
	UPROPERTY(config, EditAnywhere)
		bool bConnectInEditorRemote = false;

#endif //WITH_EDITORONLY_DATA

	/**
	* Whether to automatically create a new session after player login.
	*/
	UPROPERTY(config, EditAnywhere)
		bool bAutoCreateSession = true;

	/**
	* Whether to automatically fetch actions after player login.
	*/
	UPROPERTY(config, EditAnywhere)
		bool bAutoFetchActions = true;

	/**
	* Whether to automatically fetch types after player login.
	*/
	UPROPERTY(config, EditAnywhere)
		bool bAutoFetchTypes = true;

	/**
	* Server address to connect to.
	* This address is ignored in PIE testing unless specifically enabled.
	*/
	UPROPERTY(config, EditAnywhere)
		FString ServerAddress = "localhost";

	/**
	* Server port to connect to.
	* This is also used to define the server port if server is started locally.
	*/
	UPROPERTY(config, EditAnywhere)
		int32 ServerPort = 3000;

	/**
	* Time in seconds between server updates.
	*/
	UPROPERTY(config, EditAnywhere)
		float UpdateFrequency = 2.0f;

	/**
	* Maximum delay in seconds we expect
	* TODO: Measure ping and dynamically adapt
	*/
	UPROPERTY(config, EditAnywhere)
		float MaxDelay = 0.2f;
};


