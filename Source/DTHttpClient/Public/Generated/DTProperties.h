
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTProperties.generated.h"

UENUM(BlueprintType)
enum class EDTPropertyType : uint8
{
    /**  */
    Boolean,
    /**  */
    Number,
    /**  */
    String,
    /**  */
    SceneReg,
    /**  */
    EntityRef,
    /**  */
    LevelAsset,
    /**  */
    ActorAsset,
    /**  */
    StaticMeshAsset,
    /**  */
    SkeletalMeshAsset,
    /**  */
    TextureAsset,
    /**  */
    AnimationAsset,
    /**  */
    Transform,
    /**  */
    BooleanArray,
    /**  */
    NumberArray,
    /**  */
    StringArray,
    /**  */
    Type,
    Any
};

FORCEINLINE FString LexToString(EDTPropertyType Value)
{
    switch (Value)
    {
    case EDTPropertyType::Boolean: return TEXT("Boolean");
    case EDTPropertyType::Number: return TEXT("Number");
    case EDTPropertyType::String: return TEXT("String");
    case EDTPropertyType::SceneReg: return TEXT("SceneReg");
    case EDTPropertyType::EntityRef: return TEXT("EntityRef");
    case EDTPropertyType::LevelAsset: return TEXT("LevelAsset");
    case EDTPropertyType::ActorAsset: return TEXT("ActorAsset");
    case EDTPropertyType::StaticMeshAsset: return TEXT("StaticMeshAsset");
    case EDTPropertyType::SkeletalMeshAsset: return TEXT("SkeletalMeshAsset");
    case EDTPropertyType::TextureAsset: return TEXT("TextureAsset");
    case EDTPropertyType::AnimationAsset: return TEXT("AnimationAsset");
    case EDTPropertyType::Transform: return TEXT("Transform");
    case EDTPropertyType::BooleanArray: return TEXT("BooleanArray");
    case EDTPropertyType::NumberArray: return TEXT("NumberArray");
    case EDTPropertyType::StringArray: return TEXT("StringArray");
    case EDTPropertyType::Type: return TEXT("Type");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTPropertyType& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("Boolean"))) Value = EDTPropertyType::Boolean; else
    if (Name.Equals(TEXT("Number"))) Value = EDTPropertyType::Number; else
    if (Name.Equals(TEXT("String"))) Value = EDTPropertyType::String; else
    if (Name.Equals(TEXT("SceneReg"))) Value = EDTPropertyType::SceneReg; else
    if (Name.Equals(TEXT("EntityRef"))) Value = EDTPropertyType::EntityRef; else
    if (Name.Equals(TEXT("LevelAsset"))) Value = EDTPropertyType::LevelAsset; else
    if (Name.Equals(TEXT("ActorAsset"))) Value = EDTPropertyType::ActorAsset; else
    if (Name.Equals(TEXT("StaticMeshAsset"))) Value = EDTPropertyType::StaticMeshAsset; else
    if (Name.Equals(TEXT("SkeletalMeshAsset"))) Value = EDTPropertyType::SkeletalMeshAsset; else
    if (Name.Equals(TEXT("TextureAsset"))) Value = EDTPropertyType::TextureAsset; else
    if (Name.Equals(TEXT("AnimationAsset"))) Value = EDTPropertyType::AnimationAsset; else
    if (Name.Equals(TEXT("Transform"))) Value = EDTPropertyType::Transform; else
    if (Name.Equals(TEXT("BooleanArray"))) Value = EDTPropertyType::BooleanArray; else
    if (Name.Equals(TEXT("NumberArray"))) Value = EDTPropertyType::NumberArray; else
    if (Name.Equals(TEXT("StringArray"))) Value = EDTPropertyType::StringArray; else
    if (Name.Equals(TEXT("Type"))) Value = EDTPropertyType::Type; else
    { Value = EDTPropertyType::Any; }
};
