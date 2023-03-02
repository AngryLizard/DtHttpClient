
#pragma once

#include "CoreMinimal.h"
#include "DTTransform.generated.h"


USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FDTTransform
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float X = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Y = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float R = 0.f;
};

FORCEINLINE FString LexToString(const FDTTransform& Value)
{
	return FString::Printf(TEXT("(%.3f,%.3f,%.3f)"), Value.X, Value.Y, Value.R);
}

FORCEINLINE void LexFromString(FDTTransform& Value, const TCHAR* Buffer)
{
	TArray<FString> Array;
	FString(Buffer)
		.Replace(TEXT("("), TEXT(""))
		.Replace(TEXT(")"), TEXT(""))
		.ParseIntoArray(Array, TEXT(","), true);

	if (Array.Num() != 3)
	{
		Value = FDTTransform();
		return;
	}

	Value.X = FCString::Atoi(*Array[0]);
	Value.Y = FCString::Atoi(*Array[1]);
	Value.R = FCString::Atoi(*Array[2]);
}