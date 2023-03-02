
#pragma once

#include "CoreMinimal.h"
#include "DTTypeInfo.generated.h"

USTRUCT(BlueprintType)
struct FTypePropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Type = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Initial = "";
};

USTRUCT(BlueprintType)
struct FTypeInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Asset = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Parent = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FTypePropInfo> Props;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FTypePropInfo> Attrs;
};

USTRUCT(BlueprintType)
struct FTypeSetup
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FTypeInfo> Infos;
};
