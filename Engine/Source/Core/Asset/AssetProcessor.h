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
        nodeType = "Core/AssetProcessor";
        inputs = {
            "assetPath",     // Path to the asset
            "assetData",     // Asset data
            "assetMetadata"  // Asset metadata
        };
        outputs = {
            "processedAsset",   // Processed asset
            "assetStatus",   // Asset processing status
            "assetMetadata"  // Asset metadata
        }
    }
};

class AssetProcessor : public Node {
public:
    explicit AssetProcessor(const AssetProcessorInfo& info = AssetProcessorInfo())
        : Node(info), AssetProcessorInfo(info) {}
    void initialize();

    enum class OptimizationLevel {
        Low,
        Medium,
        High
    };

    // Set default values
    // (...)

    // === Allocation, Initialization, Loading ===
    void enableAsyncProcessing(bool enable);
    void setMaxConcurrentTasks(int maxTasks);
    void setOptimizationLevel(OptimizationLevel level);
    void addStage(const std::string& stageName, const std::string& stageType);
    void processAsset(const std::string& assetPath);

    // === Processing ===
    void () override;
    void update();

    // === Cleanup ===
    ~AssetProcessor() = default;
};

} // namespace hd
