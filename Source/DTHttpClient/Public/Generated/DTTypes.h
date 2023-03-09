
#pragma once

#include "CoreMinimal.h"
#include "DTTypes.generated.h"

UENUM(BlueprintType)
enum class EDTType : uint8
{
    Invalid,
    Entity,
    Actor,
    Character,
    Yoyo,
    Spawner,
    Item,
    World
};

FORCEINLINE FString LexToString(EDTType Value)
{
    switch (Value)
    {
    case EDTType::Entity: return TEXT("Entity");
    case EDTType::Actor: return TEXT("Actor");
    case EDTType::Character: return TEXT("Character");
    case EDTType::Yoyo: return TEXT("Yoyo");
    case EDTType::Spawner: return TEXT("Spawner");
    case EDTType::Item: return TEXT("Item");
    case EDTType::World: return TEXT("World");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTType& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("Entity"))) Value = EDTType::Entity; else
    if (Name.Equals(TEXT("Actor"))) Value = EDTType::Actor; else
    if (Name.Equals(TEXT("Character"))) Value = EDTType::Character; else
    if (Name.Equals(TEXT("Yoyo"))) Value = EDTType::Yoyo; else
    if (Name.Equals(TEXT("Spawner"))) Value = EDTType::Spawner; else
    if (Name.Equals(TEXT("Item"))) Value = EDTType::Item; else
    if (Name.Equals(TEXT("World"))) Value = EDTType::World; else
    { Value = EDTType::Invalid; }
};
