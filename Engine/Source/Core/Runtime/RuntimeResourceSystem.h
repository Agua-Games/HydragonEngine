/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime resource loading and management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeResourceConfig {
    bool enableAsyncLoading = true;
    bool enableResourcePools = true;
    bool enableHotReload = true;
    bool enableDefragmentation = true;
    uint32_t maxConcurrentLoads = 8;
    uint32_t poolGrowthFactor = 2;
    size_t defaultPoolSize = 64 * 1024 * 1024;  // 64MB
};

class RuntimeResourceSystem {
public:
    static RuntimeResourceSystem& Get();
    
    void Initialize(const RuntimeResourceConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ResourceHandle LoadResource(const ResourceDesc& desc);
    void UnloadResource(ResourceHandle handle);
    
    void CreateResourcePool(const std::string& name, size_t initialSize);
    void DestroyResourcePool(const std::string& name);
    
    void SetResourcePriority(ResourceHandle handle, ResourcePriority priority);
    ResourceState GetResourceState(ResourceHandle handle) const;
    
    const RuntimeResourceStats& GetStats() const { return m_Stats; }

private:
    RuntimeResourceSystem() = default;
    
    RuntimeResourceConfig m_Config;
    RuntimeResourceStats m_Stats;
    std::unordered_map<std::string, std::unique_ptr<IResourcePool>> m_Pools;
    std::unordered_map<ResourceHandle, ResourceInstance> m_Resources;
    std::unique_ptr<IResourceLoader> m_Loader;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 