// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GFTemplateSample : ModuleRules
{
	public GFTemplateSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GFTemplateSample",
			"GFTemplateSample/Variant_Platforming",
			"GFTemplateSample/Variant_Platforming/Animation",
			"GFTemplateSample/Variant_Combat",
			"GFTemplateSample/Variant_Combat/AI",
			"GFTemplateSample/Variant_Combat/Animation",
			"GFTemplateSample/Variant_Combat/Gameplay",
			"GFTemplateSample/Variant_Combat/Interfaces",
			"GFTemplateSample/Variant_Combat/UI",
			"GFTemplateSample/Variant_SideScrolling",
			"GFTemplateSample/Variant_SideScrolling/AI",
			"GFTemplateSample/Variant_SideScrolling/Gameplay",
			"GFTemplateSample/Variant_SideScrolling/Interfaces",
			"GFTemplateSample/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
