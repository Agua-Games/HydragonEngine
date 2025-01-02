/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset editing and management system
 */

#pragma once
#include "AssetTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct AssetEditorConfig {
    bool enableVersionControl = true;
    bool enableAssetTracking = true;
    bool enablePreviewGeneration = true;
    bool enableBatchProcessing = true;
    uint32_t maxRecentAssets = 50;
    size_t previewCacheSize = 256 * 1024 * 1024;  // 256MB
};

class AssetEditorSystem {
public:
    static AssetEditorSystem& Get();
    
    void Initialize(const AssetEditorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void OpenAsset(const std::string& path);
    void SaveAsset(const std::string& path);
    void CloseAsset(const std::string& path);
    
    void ImportAsset(const std::string& sourcePath, const ImportOptions& options);
    void ExportAsset(const std::string& path, const ExportOptions& options);
    
    void GeneratePreview(const std::string& path);
    void InvalidatePreview(const std::string& path);
    
    void RegisterAssetType(const AssetTypeInfo& info);
    void UnregisterAssetType(const std::string& type);
    
    const AssetEditorStats& GetStats() const { return m_Stats; }

private:
    AssetEditorSystem() = default;
    
    AssetEditorConfig m_Config;
    AssetEditorStats m_Stats;
    std::unordered_map<std::string, AssetEditor> m_Editors;
    std::unique_ptr<IAssetProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 