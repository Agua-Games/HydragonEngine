/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Resource management system
 */

#pragma once
#include "ResourceTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Resource {

class ResourceSystem {
public:
    struct ResourceConfig {
        uint32_t maxResources = 10000;
        uint32_t maxPendingLoads = 1000;
        bool enableAsyncLoading = true;
        bool enableHotReload = true;
        bool enableResourcePacking = true;
        size_t memoryBudget = 512 * 1024 * 1024;  // 512MB
        std::string resourceRootPath;
    };

    static ResourceSystem& Get();
    
    void Initialize(const ResourceConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ResourceHandle LoadResource(const ResourceDesc& desc);
    void UnloadResource(ResourceHandle handle);
    
    void* GetResourceData(ResourceHandle handle);
    ResourceState GetResourceState(ResourceHandle handle) const;
    
    void SetResourceCallback(ResourceHandle handle, const ResourceCallback& callback);
    float GetLoadProgress(ResourceHandle handle) const;
    
    void PreloadResource(const std::string& path);
    void PurgeUnusedResources();
    
    const ResourceStats& GetStats() const { return m_Stats; }

private:
    ResourceSystem() = default;
    
    ResourceConfig m_Config;
    ResourceStats m_Stats;
    std::unique_ptr<IResourceLoader> m_Loader;
    std::vector<Resource> m_Resources;
    bool m_Initialized = false;
};

} // namespace Hydragon::Resource 