
#pragma once

#include "CoreMinimal.h"
#include "DTTypes.generated.h"

UENUM(BlueprintType)
enum class EDTType : uint8
{
    Invalid,
    Entity,
    World,
    Actor,
    Character,
    Yoyo,
    Spawner,
    Item
};

FORCEINLINE FString LexToString(EDTType Value)
{
    switch (Value)
    {
    case EDTType::Entity: return TEXT("Entity");
    case EDTType::World: return TEXT("World");
    case EDTType::Actor: return TEXT("Actor");
    case EDTType::Character: return TEXT("Character");
    case EDTType::Yoyo: return TEXT("Yoyo");
    case EDTType::Spawner: return TEXT("Spawner");
    case EDTType::Item: return TEXT("Item");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTType& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("Entity"))) Value = EDTType::Entity; else
    if (Name.Equals(TEXT("World"))) Value = EDTType::World; else
    if (Name.Equals(TEXT("Actor"))) Value = EDTType::Actor; else
    if (Name.Equals(TEXT("Character"))) Value = EDTType::Character; else
    if (Name.Equals(TEXT("Yoyo"))) Value = EDTType::Yoyo; else
    if (Name.Equals(TEXT("Spawner"))) Value = EDTType::Spawner; else
    if (Name.Equals(TEXT("Item"))) Value = EDTType::Item; else
    { Value = EDTType::Invalid; }
};
