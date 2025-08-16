using UnrealBuildTool;

public class DedicatedServers : ModuleRules
{
	public DedicatedServers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameLiftServerSDK",
			"GameplayTags",
			"HTTP",
			"InputCore",
			"NetCore",
			"EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG",
			"Json", "JsonUtilities"
		});
		
		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "MessageLog" });
		}
	}
}
