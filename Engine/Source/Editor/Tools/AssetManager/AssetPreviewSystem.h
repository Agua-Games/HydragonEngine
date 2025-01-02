/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset preview generation and caching system
 */

#pragma once
#include "AssetTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct AssetPreviewConfig {
    bool enableBackgroundGeneration = true;
    bool enableCaching = true;
    bool enableCompression = true;
    bool enableMipMaps = true;
    uint32_t maxPreviewSize = 512;
    uint32_t maxConcurrentPreviews = 4;
    size_t previewCacheSize = 128 * 1024 * 1024;  // 128MB
};

class AssetPreviewSystem {
public:
    static AssetPreviewSystem& Get();
    
    void Initialize(const AssetPreviewConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RequestPreview(const std::string& assetPath);
    void CancelPreview(const std::string& assetPath);
    
    void SetPreviewSize(uint32_t size);
    void SetPreviewQuality(PreviewQuality quality);
    
    void RegisterPreviewGenerator(const std::string& assetType, 
                                std::unique_ptr<IPreviewGenerator> generator);
    
    PreviewHandle GetPreview(const std::string& assetPath) const;
    bool IsPreviewReady(const std::string& assetPath) const;
    
    void InvalidatePreview(const std::string& assetPath);
    void ClearPreviewCache();
    
    const AssetPreviewStats& GetStats() const { return m_Stats; }

private:
    AssetPreviewSystem() = default;
    
    AssetPreviewConfig m_Config;
    AssetPreviewStats m_Stats;
    std::unordered_map<std::string, PreviewData> m_Previews;
    std::unique_ptr<IPreviewCache> m_Cache;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 