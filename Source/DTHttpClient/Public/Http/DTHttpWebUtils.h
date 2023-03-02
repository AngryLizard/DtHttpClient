
#pragma once

#include "CoreMinimal.h"

FORCEINLINE FString SanitizeArg(const FString& Arg)
{
	FString String = Arg;
	String.RemoveFromStart("\"");
	String.RemoveFromEnd("\"");
	return String;
}

template<typename T>
FORCEINLINE FString QueryListArgs(const FString& Name, const TArray<T>& Args)
{
	const int32 Num = Args.Num();
	if (Num == 0)
	{
		return {};
	}

	FString String = SanitizeArg(LexToString(Args[0]));
	for (int32 Index = 1; Index < Num; Index++)
	{
		String += "&" + Name + "=" + SanitizeArg(LexToString(Args[Index]));
	}
	return String;
}

FORCEINLINE FString LexToString(const FDateTime& value)
{
	return value.ToIso8601();
}

FORCEINLINE void LexFromString(FDateTime& Value, const TCHAR* Buffer)
{
	FDateTime::Parse(Buffer, Value);
}

template<typename T>
FORCEINLINE FString LexToString(const TArray<T>& Values)
{
	FString Out = "[";
	if (Values.Num() > 0)
	{
		Out += LexToString(Values[0]);
		for (int32 Index = 1; Index < Values.Num(); Index++)
		{
			Out += "," + FString(LexToString(Values[Index]));
		}
	}
	Out += "]";
	return *Out;
}

template<typename T>
FORCEINLINE FString LexToString(const TMap<FString, T>& Values)
{
	FString Out = "{";
	for (const auto& Pair : Values)
	{
		Out += LexToString(Pair.Key) + "=" + LexToString(Pair.Value) + ",";
	}
	Out += "}";
	return *Out;
}
