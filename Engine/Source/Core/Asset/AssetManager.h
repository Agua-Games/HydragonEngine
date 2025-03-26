/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AssetManager.h
 * @brief Header file for the AssetManager class.
 * To avoid confusion: the AssetManager is the engine's asset manager, not the AssetEditor (UI), nor the Asset node, which loads an individual asset.
 * 
 * ARCHITECTURAL NOTES:
 * - AssetManager is a singleton class that manages the loading and management of assets (which are nodes).
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <filesystem>
#include "Node.h"
#include "Asset.h"

namespace hd {

struct AssetManagerInfo : public NodeInfo {
    AssetManagerInfo() {
        nodeType = "Core/AssetManager";
        inputs = {
            
        };
        outputs = {
            "loadedAsset",   // Loaded asset
            "assetStatus",   // Asset loading status
            "assetMetadata"  // Asset metadata
        }
    }
};

class AssetManager : public Node {
public:
    explicit AssetManager(const AssetManagerInfo& info = AssetManagerInfo())
        : Node(info), AssetManagerInfo(info) {}
    void initialize();

    // Set default values
    // (...)

    // === Allocation, Initialization, Loading ===
    void streamAsset(const std::string& assetPath);

    // === Processing ===
    void () override;
    void update();

    // === Cleanup ===
    ~AssetManager() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<Asset>> assets;
};

} // namespace hd