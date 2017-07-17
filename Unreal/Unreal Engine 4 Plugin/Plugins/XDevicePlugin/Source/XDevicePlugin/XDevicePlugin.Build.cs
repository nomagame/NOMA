// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class XDevicePlugin : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    private string BinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
    }

	public XDevicePlugin(TargetInfo Target)
	{
		PublicIncludePaths.AddRange(
			new string[] {
                "XDevicePlugin/Public",
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				"XDevicePlugin/Private",
			}
			);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
                "Engine",
                "InputCore",
                "InputDevice",
                "HeadMountedDisplay",
                "Projects",
                "Slate",
                "SlateCore"
				// ... add other public dependencies that you statically link with here ...
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...
			}
			);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        LoadXDeviceLib(Target);

        //Add DLL for packaging

	}

    public bool LoadXDeviceLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;

		if(Target.Platform==UnrealTargetPlatform.Android) {
            isLibrarySupported = true;
			
			string AndroidBaseLibPath = Path.Combine(BinariesPath, "Android/xdevice-api_library/libs/");
			string AndroidArmLibPath = AndroidBaseLibPath + "armeabi-v7a/";

			// toolchain will filter properly
			PublicLibraryPaths.Add(AndroidArmLibPath);
			PublicAdditionalLibraries.Add("xdevice");
			PrivateDependencyModuleNames.Add("Launch");
		}else 
		if((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

			//Lib based bind unsupported due to xdevice wrong lib version compile, limiting platforms to windows 32/64
			//We use a direct dll bind, with data format from .lib

			if (Target.Platform == UnrealTargetPlatform.Win64)
            {
				
				//PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "Win64", "xdevice.lib"));
				//
                PublicDelayLoadDLLs.Add("xdevice.dll");
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(BinariesPath, "Win64", "xdevice.dll")));
				// X-Device TODO : Add dll dependencies here.
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(BinariesPath, "Win64", "bleapi.dll")));
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(BinariesPath, "Win64", "wcl04.dll")));
            }
            else
            {
				// X-Device TODO : Add dll dependencies here.
            }
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Ximmerse", "Include"));
        }


        return isLibrarySupported;
    }
}

