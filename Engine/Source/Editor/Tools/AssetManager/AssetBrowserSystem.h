/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Content browser and asset organization system
 */

#pragma once
#include "AssetTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct AssetBrowserConfig {
    bool enableFiltering = true;
    bool enableSorting = true;
    bool enableDragDrop = true;
    bool enableQuickSearch = true;
    uint32_t maxRecentItems = 50;
    uint32_t thumbnailCacheSize = 100;
    std::string defaultViewMode = "Grid";
};

class AssetBrowserSystem {
public:
    static AssetBrowserSystem& Get();
    
    void Initialize(const AssetBrowserConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void SetCurrentDirectory(const std::string& path);
    std::string GetCurrentDirectory() const;
    
    void RefreshView();
    void SetViewMode(const std::string& mode);
    
    void SetFilter(const AssetFilter& filter);
    void ClearFilter();
    
    void SetSortMode(AssetSortMode mode, bool ascending);
    
    void SelectAsset(const std::string& path);
    void DeselectAsset(const std::string& path);
    void ClearSelection();
    
    void RegisterContextMenu(const std::string& assetType, 
                           const ContextMenuCallback& callback);
    
    const AssetBrowserStats& GetStats() const { return m_Stats; }

private:
    AssetBrowserSystem() = default;
    
    AssetBrowserConfig m_Config;
    AssetBrowserStats m_Stats;
    std::vector<std::string> m_Selection;
    std::unique_ptr<IAssetView> m_View;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 