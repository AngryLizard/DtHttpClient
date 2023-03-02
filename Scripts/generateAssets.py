import json
import sys
import os 

from config import getAssetCType, getAssetTag

dir_path = os.path.dirname(os.path.realpath(__file__))

sourcePath = os.path.join(dir_path, "../NodeJS/Server/build/generated", "assets.json")
targetPath = os.path.join(dir_path, "../Source/DTHttpClient/Public/Generated")

if not os.path.exists(sourcePath):
    sys.exit(f'Source path /"{sourcePath}/" not found')

sourceFile = open(sourcePath)
sourceJson = json.load(sourceFile)

def sanitizeName(name):
    if len(name) == 0: return ""
    return name[0].upper() + name[1:]

# Print settings
assetsData = f'''
#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Utils/DTAssetTable.h"
#include "Game/DTGameFactories.h"
#include "DTAssets.generated.h"
'''

assetsData += f'''
FORCEINLINE void AddAssets(TMap<FString, FString>& Assets)
{{'''
for asset in sourceJson['assets']:
    assetsData += f'''
    Assets.Emplace(TEXT("{asset['path']}"), TEXT("{asset['type']}"));'''
assetsData += f'''
}}
'''


for assetType in sourceJson['types']:

    cType = getAssetCType(assetType)
    tag = getAssetTag(assetType)

    assetsData += f'''
USTRUCT(BlueprintType)
struct FDT{assetType}Asset
{{
	GENERATED_USTRUCT_BODY()

    // This is not BlueprintReadWrite on purpose to uphold the category restriction, this should only ever be edited as a property.
	/** Gameplay tag representing this asset. */
	UPROPERTY(EditAnywhere, meta = (Categories = "{tag}"))
	    FGameplayTag Value;

	bool operator==(const FDT{assetType}Asset& Other) const
	{{
		return Value == Other.Value;
	}}
}};

FORCEINLINE uint32 GetTypeHash(const FDT{assetType}Asset& Asset)
{{
	return GetTypeHash(Asset.Value);
}}

FORCEINLINE FString LexToString(const FDT{assetType}Asset& Value)
{{
	return LexToString(Value.Value.ToString());
}}

FORCEINLINE void LexFromString(FDT{assetType}Asset& Value, const TCHAR* Buffer)
{{
	FName TagName;
	LexFromString(TagName, Buffer);
	if (TagName.IsNone()) Value.Value = FGameplayTag();
    else Value.Value = FGameplayTag::RequestGameplayTag(TagName, true);
}}

USTRUCT(BlueprintType)
struct FDT{assetType}AssetRow : public FTableRowBase
{{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "{tag}"))
		FGameplayTag AssetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "{assetType}")
		{cType} Value;
}};
'''

assetsData += f'''
UCLASS()
class DTHTTPCLIENT_API UDTAssetSubsystem : public UGameInstanceSubsystem
{{
	GENERATED_BODY()

public:
	static UDTAssetSubsystem* Get(UGameInstance* GameInstance)
	{{
		check(GameInstance);
		return GameInstance->GetSubsystem<UDTAssetSubsystem>();
	}}'''

for assetType in sourceJson['types']:

    cType = getAssetCType(assetType)
    assetsData += f'''
	UPROPERTY()
		TMap<FDT{assetType}Asset, {cType}> {assetType}Assets;
'''
    
assetsData += f'''
	virtual void Initialize(FSubsystemCollectionBase& Collection) override
    {{'''

for assetType in sourceJson['types']:

    assetsData += f'''
	    GetAssetsFromTables<FDT{assetType}AssetRow>({assetType}Assets, TEXT("{assetType}"));'''
    
assetsData += f'''
    }}
}};
'''


assetsTargetPath = os.path.join(targetPath, "DTAssets.h")
f = open(assetsTargetPath, "w")
f.write(assetsData)
f.close()

print(f'Generated Assets to {assetsTargetPath}')