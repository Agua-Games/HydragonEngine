/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor asset management and organization system
 */

#pragma once
#include "AssetTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

namespace Hydragon::Editor {

struct AssetManagerConfig {
    bool enableContentBrowser = true;
    bool enableAssetIndexing = true;
    bool enableMetadataCache = true;
    bool enableThumbnails = true;
    uint32_t maxConcurrentImports = 4;
    uint32_t thumbnailSize = 256;
    size_t metadataCacheSize = 64 * 1024 * 1024;  // 64MB
    std::filesystem::path assetRoot = "Content";
};

class AssetManagerSystem {
public:
    static AssetManagerSystem& Get();
    
    void Initialize(const AssetManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void CreateFolder(const std::string& path);
    void DeleteFolder(const std::string& path);
    void RenameFolder(const std::string& oldPath, const std::string& newPath);
    
    void MoveAsset(const std::string& sourcePath, const std::string& targetPath);
    void CopyAsset(const std::string& sourcePath, const std::string& targetPath);
    void DeleteAsset(const std::string& path);
    
    void ImportAssets(const std::vector<std::string>& paths);
    void ExportAssets(const std::vector<std::string>& paths, const std::string& targetDir);
    
    void BuildAssetIndex();
    void SearchAssets(const AssetSearchQuery& query, AssetSearchResults& results);
    
    void RegisterAssetProcessor(const std::string& extension, 
                              std::unique_ptr<IAssetProcessor> processor);
    
    const AssetManagerStats& GetStats() const { return m_Stats; }

private:
    AssetManagerSystem() = default;
    
    AssetManagerConfig m_Config;
    AssetManagerStats m_Stats;
    std::unordered_map<std::string, AssetInfo> m_AssetIndex;
    std::unique_ptr<IAssetDatabase> m_Database;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 