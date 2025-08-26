// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PPGame : ModuleRules
{
	public PPGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "PPGame"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "NavigationSystem",
                "AIModule",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "ModularGameplay",
                "GameFeatures",
                "Niagara",
                "PhysicsCore",
                "TraceLog",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "InputCore",
                "UMG",
                "GameplayMessageRuntime",
                "Slate",
                "SlateCore",
                "AudioExtensions",
                "DeveloperSettings",
                "EnhancedInput",
                "CommonUI"
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
            }
        );

        // Generate compile errors if using DrawDebug functions in test/shipping builds.
        PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
    }
}
