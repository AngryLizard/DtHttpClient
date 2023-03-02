
#pragma once

#include "CoreMinimal.h"
#include "DTHttpClient.h"
#include "AssetRegistry/AssetRegistryModule.h"

template<typename AssetRowType, typename AssetType, typename ValueType>
void GetAssetsFromTables(TMap<AssetType, ValueType>& Assets, const FString& Name)
{
	Assets.Reset();
	const FString Context = FString::Printf(TEXT("UDTHttpAssetSubsystem::GetAssets::%s"), *Name);

	TArray<FAssetData> AssetDatas;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	AssetRegistry.GetAssetsByClass(UDataTable::StaticClass()->GetClassPathName(), AssetDatas);
	for (const FAssetData& AssetData : AssetDatas)
	{
		UDataTable* AssetTable = Cast<UDataTable>(AssetData.GetAsset());
		if (IsValid(AssetTable) && AssetTable->GetRowStruct() == AssetRowType::StaticStruct())
		{
			TArray<AssetRowType*> Rows;
			AssetTable->GetAllRows<AssetRowType>(Context, Rows);

			for (const AssetRowType* Row : Rows)
			{
				AssetType Asset;
				Asset.Value = Row->AssetTag;
				Assets.Emplace(Asset, Row->Value);

				UE_LOG(DTHttpClient, Log, TEXT("Loaded asset %s for type %s"), *Row->AssetTag.ToString(), *Name);
			}
		}
	}
}