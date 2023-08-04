// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PF_MagicKnight : ModuleRules
{
	public PF_MagicKnight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"EnhancedInput", 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"AnimGraphRuntime",
            "NavigationSystem", 
			"AIModule"
        });
    }
}
