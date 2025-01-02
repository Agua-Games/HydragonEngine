/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset loader system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Asset/AssetLoaderSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AssetLoaderBindingsConfig {
    bool enableBackgroundLoading = true;
    bool enablePreloading = true;
    bool enableLoadPriorities = true;
    bool enableLoadGroups = true;
    uint32_t maxConcurrentLoads = 8;
    uint32_t maxLoadGroups = 32;
    uint32_t loadBufferSize = 8 * 1024 * 1024;  // 8MB
    std::string loadCachePath = "Cache/AssetLoads";
};

class AssetLoaderBindings {
public:
    static AssetLoaderBindings& Get();
    
    void Initialize(const AssetLoaderBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Load management
    LoadHandle QueueLoad(const std::string& path, LoadPriority priority = LoadPriority::Normal);
    void CancelLoad(LoadHandle handle);
    bool IsLoadComplete(LoadHandle handle) const;
    float GetLoadProgress(LoadHandle handle) const;
    
    // Load groups
    GroupHandle CreateLoadGroup(const std::string& name);
    void DestroyLoadGroup(GroupHandle handle);
    void AddToGroup(GroupHandle group, LoadHandle load);
    void RemoveFromGroup(GroupHandle group, LoadHandle load);
    bool IsGroupComplete(GroupHandle handle) const;
    float GetGroupProgress(GroupHandle handle) const;
    
    // Preloading
    void PreloadAsset(const std::string& path);
    void PreloadGroup(const std::vector<std::string>& paths);
    void CancelPreload(const std::string& path);
    
    // Load callbacks
    void SetLoadCallback(LoadHandle handle, const LoadCallback& callback);
    void SetGroupCallback(GroupHandle handle, const GroupCallback& callback);
    void SetGlobalLoadCallback(const GlobalLoadCallback& callback);
    
    // Load utilities
    void SetLoadPriority(LoadHandle handle, LoadPriority priority);
    void PauseLoading(bool pause);
    void CancelAllLoads();
    void FlushLoadCache();
    
    // Debug utilities
    void EnableLoadProfiling(bool enable);
    void DumpLoadStats(const std::string& path);
    void ValidateLoadQueue();
    
    const AssetLoaderBindingsStats& GetStats() const { return m_Stats; }

private:
    AssetLoaderBindings() = default;
    
    AssetLoaderBindingsConfig m_Config;
    AssetLoaderBindingsStats m_Stats;
    std::unordered_map<LoadHandle, LoadInstance> m_Loads;
    std::unordered_map<GroupHandle, LoadGroupInstance> m_LoadGroups;
    std::unordered_map<std::string, PreloadInfo> m_Preloads;
    std::unique_ptr<Asset::ILoadProcessor> m_Processor;
    std::unique_ptr<Asset::ILoadScheduler> m_Scheduler;
    std::unique_ptr<Asset::ILoadCache> m_LoadCache;
    std::unique_ptr<Asset::ILoadProfiler> m_Profiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 