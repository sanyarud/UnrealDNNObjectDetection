// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnrealBuildTool;

public class ObjectDetection : ModuleRules
{
    public bool IsDebug(ReadOnlyTargetRules Target)
    {
        return Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT;
    }

    public void loadAllStaticLibsFromDir(string libsPath)
    {
        string[] libs = Directory.GetFiles(libsPath, "*.lib");
        foreach (string lib in libs)
        {
            PublicAdditionalLibraries.Add(lib);
        }
    }

    public void LoadThirdPartyLibrary(ReadOnlyTargetRules Target, string libName)
    {
        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty"));
        string libPath = Path.Combine(ThirdPartyPath, libName);
        string libIncludePath = Path.Combine(libPath, "include");

        PublicIncludePaths.Add(libIncludePath);

        string libBinariesPath = Path.Combine(libPath, "lib");
        loadAllStaticLibsFromDir(libBinariesPath);
    }

    public ObjectDetection(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Add OpenCV
        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty"));
        PublicIncludePaths.Add(ThirdPartyPath);
        LoadThirdPartyLibrary(Target, "OpenCV");

        // Add CUDA
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
        string CudaPath = Environment.GetEnvironmentVariable("CUDA_PATH");
        string CudaIncludePath = Path.Combine(CudaPath, "include");
        string CudaStaticLibsPath = Path.Combine(CudaPath, "lib", PlatformString);
        string CudaDynamicLibsPath = Path.Combine(CudaPath, "bin");

        PublicIncludePaths.Add(CudaIncludePath);
        loadAllStaticLibsFromDir(CudaStaticLibsPath);
    }
}
