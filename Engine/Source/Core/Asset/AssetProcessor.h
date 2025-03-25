/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AssetProcessor.h
 * @brief Header file for the AssetProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - AssetProcessor is a class that represents an asset processor in Hydragon.
 * - It is used to process assets and their associated data, such as textures, models, shaders, audio, fonts, etc.
 * - It uses the Vulkan API for asset processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"
#include "Asset.h"

namespace hd {

struct AssetProcessorInfo : public NodeInfo {
    AssetProcessorInfo() {
        NodeType = "Core/AssetProcessor";
        inputs = {
            "AssetPath",     // Path to the asset
            "AssetData",     // Asset data
            "AssetMetadata"  // Asset metadata
        };
        outputs = {
            "ProcessedAsset",   // Processed asset
            "AssetStatus",   // Asset processing status
            "AssetMetadata"  // Asset metadata
        }
    }
};

class AssetProcessor : public Node {
public:
    explicit AssetProcessor(const AssetProcessorInfo& info = AssetProcessorInfo())
        : Node(info), AssetProcessorInfo(info) {}
    void initialize();

    // === Allocation, Initialization, Loading ===
    void processAsset(const std::string& assetPath);

    // === Processing ===
    void processNodeGraph() override;
    void update();

    // === Cleanup ===
    ~AssetProcessor() = default;
};

} // namespace hd
