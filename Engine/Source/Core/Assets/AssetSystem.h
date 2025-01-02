/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core asset management system
 */

#pragma once
#include "AssetTypes.h"
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Hydragon::Assets {

struct AssetDependencyGraph {
    std::vector<AssetHandle> dependencies;
    std::vector<AssetHandle> dependents;
    UpdateStrategy updateStrategy;
};

class AssetSystem {
public:
    struct AssetConfig {
        size_t virtualMemoryBudget = 8 * 1024 * 1024 * 1024ull;  // 8GB
        size_t maxLoadedAssets = 10000;
        bool enableAsyncLoading = true;
        bool enableHotReload = true;
        std::string assetRootPath;
    };

    static AssetSystem& Get();
    
    void Initialize(const AssetConfig& config = {});
    void Shutdown();

    AssetHandle LoadAsset(const std::string& path, const LoadParams& params = {});
    void UnloadAsset(AssetHandle handle);
    
    bool IsAssetLoaded(AssetHandle handle) const;
    const AssetInfo& GetAssetInfo(AssetHandle handle) const;
    const AssetStats& GetStats() const { return m_Stats; }

private:
    AssetSystem() = default;
    
    AssetConfig m_Config;
    AssetStats m_Stats;
    std::unique_ptr<Memory::VirtualMemoryManager> m_VirtualMemory;
    std::unordered_map<AssetHandle, AssetInfo> m_LoadedAssets;
    bool m_Initialized = false;
};

} // namespace Hydragon::Assets 