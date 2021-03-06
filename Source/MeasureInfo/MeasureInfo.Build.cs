// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeasureInfo : ModuleRules
{
	public MeasureInfo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"EditorStyle",
					"UnrealEd",
				}
				);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
					"InputCore",
					"Slate",
					"SlateCore",
					"RenderCore",
					"PropertyEditor",
				// ... add private dependencies that you statically link with here ...
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
