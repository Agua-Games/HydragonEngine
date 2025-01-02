/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset management and processing system
 */

#pragma once
#include "AssetTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Asset {

class AssetSystem {
public:
    struct AssetConfig {
        uint32_t maxAssets = 100000;
        uint32_t maxDependencies = 10000;
        bool enableAssetPacking = true;
        bool enableVersioning = true;
        bool enableCaching = true;
        size_t memoryBudget = 256 * 1024 * 1024;  // 256MB
        std::string assetRootPath;
        std::string cacheRootPath;
    };

    static AssetSystem& Get();
    
    void Initialize(const AssetConfig& config = {});
    void Shutdown();
    
    void Update();
    
    AssetHandle ImportAsset(const AssetImportDesc& desc);
    void ProcessAsset(AssetHandle handle);
    void ExportAsset(AssetHandle handle, const std::string& path);
    
    void AddDependency(AssetHandle dependent, AssetHandle dependency);
    void RemoveDependency(AssetHandle dependent, AssetHandle dependency);
    
    AssetMetadata GetMetadata(AssetHandle handle) const;
    void SetMetadata(AssetHandle handle, const AssetMetadata& metadata);
    
    const AssetStats& GetStats() const { return m_Stats; }

private:
    AssetSystem() = default;
    
    AssetConfig m_Config;
    AssetStats m_Stats;
    std::unique_ptr<IAssetProcessor> m_Processor;
    std::vector<Asset> m_Assets;
    bool m_Initialized = false;
};

} // namespace Hydragon::Asset 