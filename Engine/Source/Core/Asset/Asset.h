/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Asset.h
 * @brief Header file for the Asset class.
 * 
 * ARCHITECTURAL NOTES:
 * - Asset is an abstract base class for all assets in Hydragon. Each Asset (a node) is managed by the AssetManager, a singleton class.
 * - It provides a common interface for all assets in the engine, like textures, models, shaders, audio, fonts, etc.
 * - Assets may be loaded from disk, while others may be generated at runtime, procedurally or otherwise.
 * - Assets may be shared between multiple objects.
 * - Assets may be cached and asynchronously streamed to improve performance.
 * - It uses the Vulkan API for asset management.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Object.h"
#include "Node.h"

namespace hd {
    
struct AssetInfo : public NodeInfo {
    AssetInfo() {
        nodeType = "Core/Asset";
        inputs = {
            "assetPath",     // Path to the asset
            "assetData",     // Asset data
            "assetMetadata"  // Asset metadata
        };
        outputs = {
            "loadedAsset",   // Loaded asset
            "assetStatus",   // Asset loading status
            "assetMetadata"  // Asset metadata
        }
    }
};

class Asset : public Node {
public:
    explicit Asset(const AssetInfo& info = AssetInfo())
        : Node(info), AssetInfo(info) {}
    void initialize();

    // Set default values
    // (...)

    // === Allocation, Initialization, Loading ===
    void loadAsset(const std::string& assetPath);
    void unloadAsset(const std::string& assetPath);
    void reloadAsset(const std::string& assetPath);

    // === Processing ===
    void () override;
    void update();

    // === Cleanup ===
    ~Asset() = default;
};

} // namespace hd
