
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "DTHttpWebTypes.generated.h"

struct FWebError
{
};

struct FEmptyBody
{
};

FORCEINLINE FString LexToString(const FEmptyBody& value)
{
	return TEXT("");
}

FORCEINLINE void LexFromString(FEmptyBody& Value, const TCHAR* Buffer)
{
	Value = FEmptyBody();
}


struct FEmptyResult
{
};

FORCEINLINE FString LexToString(const FEmptyResult& value)
{
	return TEXT("");
}

FORCEINLINE void LexFromString(FEmptyResult& Value, const TCHAR* Buffer)
{
	Value = FEmptyResult();
}

struct FResponseString
{
	FString Body;
	int32 Code;
};

/** Serializable server response on error */
USTRUCT()
struct DTHTTPCLIENT_API FDTResponseMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString Name = "";

	UPROPERTY()
		FString Message = "";
};

FORCEINLINE void LexFromString(FDTResponseMessage& Value, const TCHAR* Buffer)
{
	FJsonObjectConverter::JsonObjectStringToUStruct<FDTResponseMessage>(Buffer, &Value);
};
