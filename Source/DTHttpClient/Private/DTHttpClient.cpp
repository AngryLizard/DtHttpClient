// Copyright Epic Games, Inc. All Rights Reserved.

#include "DTHttpClient.h"
#include "GameplayTagsManager.h"
#include "Generated/DTAssets.h"
#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY(DTHttpClient);

#define LOCTEXT_NAMESPACE "FDTHttpClientModule"

void FDTHttpClientModule::StartupModule()
{
	TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("DTHttp"));
	check(ThisPlugin.IsValid());

	UGameplayTagsManager::Get().AddTagIniSearchPath(ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags"));

	TMap<FString, FString> Assets;
	AddAssets(Assets);
	for (const auto& Asset : Assets)
	{
		const FGameplayTag GameplayTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(Asset.Key), TEXT("Generated asset path"));
		GeneratedAssets.FindOrAdd(Asset.Value, TArray<FGameplayTag>()).Emplace(GameplayTag);
	}
}

void FDTHttpClientModule::ShutdownModule()
{
	GeneratedAssets.Reset();

	TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("DTHttp"));
	check(ThisPlugin.IsValid());

	// TODO: Currently produces crash
	//UGameplayTagsManager::Get().RemoveTagIniSearchPath(ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDTHttpClientModule, DTHttpClient)