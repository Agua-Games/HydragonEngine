/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Build automation and management system
 */

#pragma once
#include "BuildTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

namespace Hydragon::DevTools {

struct BuildConfig {
    bool enableIncrementalBuild = true;
    bool enableParallelBuild = true;
    bool enableCacheSystem = true;
    bool enableDependencyTracking = true;
    uint32_t maxConcurrentJobs = 8;
    std::filesystem::path intermediateDir = "Intermediate";
    std::filesystem::path outputDir = "Build";
};

class BuildSystem {
public:
    static BuildSystem& Get();
    
    void Initialize(const BuildConfig& config = {});
    void Shutdown();
    
    BuildHandle CreateBuild(const BuildDesc& desc);
    void DestroyBuild(BuildHandle handle);
    
    void AddTarget(BuildHandle build, const BuildTarget& target);
    void RemoveTarget(BuildHandle build, const std::string& targetName);
    
    void SetBuildEnvironment(BuildHandle build, const BuildEnvironment& env);
    void AddDependency(BuildHandle build, const std::string& target, 
                      const std::string& dependency);
    
    bool BuildTarget(BuildHandle build, const std::string& targetName);
    bool BuildAll(BuildHandle build);
    void CancelBuild(BuildHandle build);
    
    bool IsBuildRunning(BuildHandle build) const;
    float GetBuildProgress(BuildHandle build) const;
    
    const BuildStats& GetStats() const { return m_Stats; }

private:
    BuildSystem() = default;
    
    BuildConfig m_Config;
    BuildStats m_Stats;
    std::vector<Build> m_Builds;
    std::unique_ptr<IBuildExecutor> m_Executor;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 