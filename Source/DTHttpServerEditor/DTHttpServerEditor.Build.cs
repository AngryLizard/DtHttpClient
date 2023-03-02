// Copyright Epic Games, Inc. All Rights Reserved.

using System.Diagnostics;
using System.IO;
using System;
using UnrealBuildTool;

public class DTHttpServerEditor : ModuleRules
{
	public DTHttpServerEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "EditorSubsystem",
                "DesktopPlatform",
                "UnrealEd",
                "Projects",
                "ToolMenus",

                "DTHttpClient"
            });

		// Rebuild nodes if ts or js files changed
        String ServerPath = Path.Combine(PluginDirectory, "NodeJS/Server");

        // TODO: Manually called via bat file for now, or this will run way too often
        //ExternalDependencies.Add(Path.Combine(ServerPath, "src/*"));
        //ExternalDependencies.Add(Path.Combine(ServerPath, "build/*"));

        //runNodeBuild(ServerPath);
        //runPythonGen();
    }

    private void runNodeBuild(string ServerPath)
    {
        String Command = $@"/C npm run --prefix {ServerPath} build";
        ProcessStartInfo Cmdsi = new ProcessStartInfo("cmd.exe");
        Cmdsi.Arguments = Command;
        Process Cmd = Process.Start(Cmdsi);
        Cmd.WaitForExit();

        if (Cmd.ExitCode != 0)
        {
            System.Environment.Exit(Cmd.ExitCode);
        }
    }

    private void runPythonGen()
    {
        String GenPath = Path.Combine(PluginDirectory, "Scripts/generateSwagger.py");
        String Command = $@"/C python3 {GenPath}";
        ProcessStartInfo Cmdsi = new ProcessStartInfo("cmd.exe");
        Cmdsi.Arguments = Command;
        Process Cmd = Process.Start(Cmdsi);
        Cmd.WaitForExit();

        if (Cmd.ExitCode != 0)
        {
            System.Environment.Exit(Cmd.ExitCode);
        }
    }
}
