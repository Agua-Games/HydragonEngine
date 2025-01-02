/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core resource management system
 */

#pragma once
#include "ResourceTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Hydragon::Resources {

class ResourceSystem {
public:
    struct ResourceConfig {
        size_t memoryBudget = 2 * 1024 * 1024 * 1024;  // 2GB
        uint32_t maxResources = 100000;
        bool enableAsyncLoading = true;
        bool enableCaching = true;
        bool enableCompression = true;
        bool enableHotReload = true;
        std::string resourceRootPath;
    };

    static ResourceSystem& Get();
    
    void Initialize(const ResourceConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ResourceHandle LoadResource(const ResourceDesc& desc);
    void UnloadResource(ResourceHandle handle);
    
    void PreloadResource(const ResourceDesc& desc);
    void SetResourcePriority(ResourceHandle handle, float priority);
    
    bool IsResourceLoaded(ResourceHandle handle) const;
    float GetLoadProgress(ResourceHandle handle) const;
    
    void* GetResourceData(ResourceHandle handle);
    const ResourceInfo& GetResourceInfo(ResourceHandle handle) const;
    
    void AddResourceCallback(ResourceHandle handle, const ResourceCallback& callback);
    void RemoveResourceCallback(ResourceHandle handle);
    
    const ResourceStats& GetStats() const { return m_Stats; }

private:
    ResourceSystem() = default;
    
    ResourceConfig m_Config;
    ResourceStats m_Stats;
    std::unique_ptr<IResourceProvider> m_Provider;
    std::unordered_map<ResourceHandle, Resource> m_Resources;
    bool m_Initialized = false;
};

} // namespace Hydragon::Resources 