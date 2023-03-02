// Copyright Epic Games, Inc. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using System;
using UnrealBuildTool;

public class DTHttpClient : ModuleRules
{
	public DTHttpClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "HTTP",
                "Json",
                "JsonUtilities",
                "DeveloperSettings",
				"GameplayTags",
                "Projects",
                "AssetRegistry",

                "AngryUtility"
			});
    }
}
