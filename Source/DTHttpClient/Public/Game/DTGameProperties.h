
#pragma once

#include "CoreMinimal.h"
#include "Utils/DTGenericTimeline.h"
#include "Utils/DTTransform.h"
#include "Setup/DTSetupObject.h"
#include "Generated/DTTypes.h"
#include "Generated/DTAssets.h"
#include "Generated/DTProperties.h"
#include "DTGameProperties.generated.h"

//////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FSerialPropertyValue
{
	GENERATED_BODY()

	// This isn't accessible by Blueprint on purpose
	FString Value = "";
	EDTPropertyType Type = EDTPropertyType::Any;
};

//////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FDTSceneRegSetup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ADTSceneActor* Value = nullptr;
};

FORCEINLINE FString LexToString(const FDTSceneRegSetup& Value)
{
	if (!IsValid(Value.Value) || Value.Value->SceneKey.Value.IsEmpty()) return "";
	return "#" + (Value.Value->SceneKey.Value);
};

//////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct DTHTTPCLIENT_API FDTEntityRefSetup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ADTSetupBaseActor* Value = nullptr;
};

FORCEINLINE FString LexToString(const FDTEntityRefSetup& Value)
{
	if (!IsValid(Value.Value)) return "";
	const FString Ptr = Value.Value->GetPtr();
	if (Ptr.IsEmpty()) return "";
	return "#" + Ptr;
};

//////////////////////////////////////////////////////////

template<typename ValueType>
struct FDTGenericPropertyValue
{
	ValueType Value;

	FDTGenericPropertyValue(const FString& String) {
		LexFromString(Value, *String);
	}
	virtual ~FDTGenericPropertyValue() {}
};