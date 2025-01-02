/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Resource system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Resource/ResourceSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct ResourceBindingsConfig {
    bool enableAsyncLoading = true;
    bool enableHotReloading = true;
    bool enableResourcePacking = true;
    bool enableCaching = true;
    uint32_t maxPendingLoads = 100;
    uint32_t maxCacheSize = 256 * 1024 * 1024;  // 256MB
    uint32_t maxPackageSize = 1024 * 1024 * 1024;  // 1GB
    std::string resourceCachePath = "Cache/Resources";
};

class ResourceBindings {
public:
    static ResourceBindings& Get();
    
    void Initialize(const ResourceBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Resource loading
    ResourceHandle LoadResource(const std::string& path, const LoadParams& params = {});
    void UnloadResource(ResourceHandle handle);
    bool IsResourceLoaded(const std::string& path) const;
    float GetLoadProgress(ResourceHandle handle) const;
    
    // Resource management
    void PreloadResource(const std::string& path);
    void SetResourcePriority(ResourceHandle handle, ResourcePriority priority);
    void ReloadResource(ResourceHandle handle);
    
    // Resource packing
    void CreateResourcePackage(const std::string& name, const std::vector<std::string>& resources);
    void LoadResourcePackage(const std::string& name);
    void UnloadResourcePackage(const std::string& name);
    
    // Resource caching
    void EnableResourceCaching(bool enable);
    void ClearResourceCache();
    void SetCacheLimit(size_t sizeInBytes);
    
    // Resource monitoring
    void RegisterLoadCallback(const std::string& path, const LoadCallback& callback);
    void UnregisterLoadCallback(const std::string& path);
    void EnableHotReload(const std::string& path, bool enable);
    
    // Resource utilities
    ResourceInfo GetResourceInfo(ResourceHandle handle) const;
    ResourceStats GetResourceStats() const;
    void DumpResourceState(const std::string& path);
    
    const ResourceBindingsStats& GetStats() const { return m_Stats; }

private:
    ResourceBindings() = default;
    
    ResourceBindingsConfig m_Config;
    ResourceBindingsStats m_Stats;
    std::unordered_map<std::string, ResourceHandle> m_ResourceMap;
    std::unordered_map<ResourceHandle, ResourceInstance> m_Resources;
    std::unordered_map<std::string, LoadCallback> m_LoadCallbacks;
    std::unordered_map<std::string, PackageInfo> m_Packages;
    std::unique_ptr<Resource::IResourceLoader> m_Loader;
    std::unique_ptr<Resource::IResourceCache> m_Cache;
    std::unique_ptr<Resource::IPackageManager> m_PackageManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 