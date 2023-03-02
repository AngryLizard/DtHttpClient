
#pragma once

#include "CoreMinimal.h"
#include "DTIdentifiers.generated.h"

USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FDTSceneReg
{
	GENERATED_USTRUCT_BODY()

	// Hidden on purpose
	FString Value = "";

	bool operator==(const FDTSceneReg& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTSceneReg& SceneReg)
{
	return GetTypeHash(SceneReg.Value);
}

FORCEINLINE TArray<FString> ToStrings(const TArray<FDTSceneReg>& SceneRegs)
{
	TArray<FString> StringArray;
	Algo::Transform(SceneRegs, StringArray, [](const FDTSceneReg& SceneReg)
		{
			return SceneReg.Value;
		});
	return StringArray;
}

FORCEINLINE FDTSceneReg ToSceneReg(const FString& String)
{
	FDTSceneReg SceneReg;
	SceneReg.Value = String;
	return SceneReg;
}

FORCEINLINE FString LexToString(const FDTSceneReg& SceneReg)
{
	return LexToString(SceneReg.Value);
}

FORCEINLINE void LexFromString(FDTSceneReg& Value, const TCHAR* Buffer)
{
	LexFromString(Value.Value, Buffer);
}

USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FDTEntityRef
{
	GENERATED_USTRUCT_BODY()

		// Hidden on purpose
		FString Value = "";

	bool operator==(const FDTEntityRef& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTEntityRef& EntityRef)
{
	return GetTypeHash(EntityRef.Value);
}

FORCEINLINE TArray<FString> ToStrings(const TArray<FDTEntityRef>& EntityRefs)
{
	TArray<FString> StringArray;
	Algo::Transform(EntityRefs, StringArray, [](const FDTEntityRef& EntityRef)
	{
		return EntityRef.Value;
	});
	return StringArray;
}

FORCEINLINE FDTEntityRef ToEntityRef(const FString& String)
{
	FDTEntityRef EntityRef;
	EntityRef.Value = String;
	return EntityRef;
}

FORCEINLINE FString LexToString(const FDTEntityRef& Value)
{
	return LexToString(Value.Value);
}

FORCEINLINE void LexFromString(FDTEntityRef& Value, const TCHAR* Buffer)
{
	LexFromString(Value.Value, Buffer);
}
