// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyGame : ModuleRules
{
	public MyGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" }); //어빌리티 시스템 사용을 위한 모듈(플러그인 체크후 지정)
    }
}
