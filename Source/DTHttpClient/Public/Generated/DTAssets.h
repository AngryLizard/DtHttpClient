
#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Utils/DTAssetTable.h"
#include "Game/DTGameFactories.h"
#include "DTAssets.generated.h"

FORCEINLINE void AddAssets(TMap<FString, FString>& Assets)
{
    Assets.Emplace(TEXT("type.factory.world"), TEXT("Factory"));
    Assets.Emplace(TEXT("type.factory.character"), TEXT("Factory"));
    Assets.Emplace(TEXT("type.entity.world.testLevel"), TEXT("Level"));
    Assets.Emplace(TEXT("type.entity.actor.box"), TEXT("Actor"));
    Assets.Emplace(TEXT("type.entity.actor.character._"), TEXT("Actor"));
    Assets.Emplace(TEXT("type.entity.actor.character.yoyo"), TEXT("Actor"));
    Assets.Emplace(TEXT("type.entity.actor.character.kirin"), TEXT("Actor"));
    Assets.Emplace(TEXT("type.mesh.characterMesh"), TEXT("SkeletalMesh"));
}

USTRUCT(BlueprintType)
struct FDTFactoryAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.factory"))
	    FGameplayTag Value;

	bool operator==(const FDTFactoryAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTFactoryAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTFactoryAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTFactoryAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTFactoryAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.factory"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Factory")
		UDTGameFactoryAsset* Value;
};

USTRUCT(BlueprintType)
struct FDTLevelAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.entity.world"))
	    FGameplayTag Value;

	bool operator==(const FDTLevelAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTLevelAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTLevelAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTLevelAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTLevelAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.entity.world"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Level")
		TSoftObjectPtr<UWorld> Value;
};

USTRUCT(BlueprintType)
struct FDTActorAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.entity.actor"))
	    FGameplayTag Value;

	bool operator==(const FDTActorAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTActorAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTActorAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTActorAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTActorAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.entity.actor"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Actor")
		TSubclassOf<AActor> Value;
};

USTRUCT(BlueprintType)
struct FDTStaticMeshAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.mesh.static"))
	    FGameplayTag Value;

	bool operator==(const FDTStaticMeshAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTStaticMeshAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTStaticMeshAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTStaticMeshAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTStaticMeshAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.mesh.static"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "StaticMesh")
		UStaticMesh* Value;
};

USTRUCT(BlueprintType)
struct FDTSkeletalMeshAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.mesh.skeletal"))
	    FGameplayTag Value;

	bool operator==(const FDTSkeletalMeshAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTSkeletalMeshAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTSkeletalMeshAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTSkeletalMeshAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTSkeletalMeshAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.mesh.skeletal"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "SkeletalMesh")
		USkeletalMesh* Value;
};

USTRUCT(BlueprintType)
struct FDTTextureAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.texture"))
	    FGameplayTag Value;

	bool operator==(const FDTTextureAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTTextureAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTTextureAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTTextureAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTTextureAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.texture"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Texture")
		UTexture* Value;
};

USTRUCT(BlueprintType)
struct FDTAnimationAsset
{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "type.animation"))
	    FGameplayTag Value;

	bool operator==(const FDTAnimationAsset& Other) const
	{
		return Value == Other.Value;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDTAnimationAsset& Asset)
{
	return GetTypeHash(Asset.Value);
}

FORCEINLINE FString LexToString(const FDTAnimationAsset& Value)
{
	return LexToString(Value.Value.ToString());
}

FORCEINLINE void LexFromString(FDTAnimationAsset& Value, const TCHAR* Buffer)
{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}

USTRUCT(BlueprintType)
struct FDTAnimationAssetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "type.animation"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Animation")
		UAnimationAsset* Value;
};

UCLASS()
class DTHTTPCLIENT_API UDTAssetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDTAssetSubsystem* Get(UGameInstance* GameInstance)
	{
		check(GameInstance);
		return GameInstance->GetSubsystem<UDTAssetSubsystem>();
	}
	UPROPERTY()
		TMap<FDTFactoryAsset, UDTGameFactoryAsset*> FactoryAssets;

	UPROPERTY()
		TMap<FDTLevelAsset, TSoftObjectPtr<UWorld>> LevelAssets;

	UPROPERTY()
		TMap<FDTActorAsset, TSubclassOf<AActor>> ActorAssets;

	UPROPERTY()
		TMap<FDTStaticMeshAsset, UStaticMesh*> StaticMeshAssets;

	UPROPERTY()
		TMap<FDTSkeletalMeshAsset, USkeletalMesh*> SkeletalMeshAssets;

	UPROPERTY()
		TMap<FDTTextureAsset, UTexture*> TextureAssets;

	UPROPERTY()
		TMap<FDTAnimationAsset, UAnimationAsset*> AnimationAssets;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
    {
	    GetAssetsFromTables<FDTFactoryAssetRow>(FactoryAssets, TEXT("Factory"));
	    GetAssetsFromTables<FDTLevelAssetRow>(LevelAssets, TEXT("Level"));
	    GetAssetsFromTables<FDTActorAssetRow>(ActorAssets, TEXT("Actor"));
	    GetAssetsFromTables<FDTStaticMeshAssetRow>(StaticMeshAssets, TEXT("StaticMesh"));
	    GetAssetsFromTables<FDTSkeletalMeshAssetRow>(SkeletalMeshAssets, TEXT("SkeletalMesh"));
	    GetAssetsFromTables<FDTTextureAssetRow>(TextureAssets, TEXT("Texture"));
	    GetAssetsFromTables<FDTAnimationAssetRow>(AnimationAssets, TEXT("Animation"));
    }
};
