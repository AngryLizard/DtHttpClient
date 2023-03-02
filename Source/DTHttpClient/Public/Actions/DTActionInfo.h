
#pragma once

#include "CoreMinimal.h"
#include "DTActionInfo.generated.h"

USTRUCT(BlueprintType)
struct FSerialPropertyValueInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Input = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Type = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Initial = "";
};

USTRUCT(BlueprintType)
struct FActionInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Domain = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Trigger = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FSerialPropertyValueInfo> Params;
};

USTRUCT(BlueprintType)
struct FActionSetup
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FActionInfo> Infos;
};