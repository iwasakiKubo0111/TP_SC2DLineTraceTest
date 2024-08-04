// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP_SC2DLineTraceTest : ModuleRules
{
	public TP_SC2DLineTraceTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore" });

    }
}
