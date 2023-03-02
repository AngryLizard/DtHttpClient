
#pragma once

#include "CoreMinimal.h"
#include "DTEntityInfo.generated.h"

USTRUCT(BlueprintType)
struct FDTAttributeInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString PropReg;

	UPROPERTY()
		FString PropVal;
};

USTRUCT(BlueprintType)
struct FDTEntityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDTEntityRef Ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDTType Type = EDTType::Invalid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> OwnerRefs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FDTAttributeInfo> Attributes;
};
