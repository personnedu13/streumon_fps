// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class streumon_fps : ModuleRules
{
	public streumon_fps(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
