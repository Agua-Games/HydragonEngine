/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MaterialProcessor.h
 * @brief Header file for the MaterialProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - MaterialProcessor is a class that represents a material processor in Hydragon.
 * - It is used to process materials and their associated data, such as material properties, textures, etc.
 * - It uses the Vulkan API for material processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Material.h"

namespace hd {

struct MaterialProcessorInfo : public AssetProcessorInfo {
    MaterialProcessorInfo() {
        nodeType = "Core/MaterialProcessor";
        inputs = {
            "materialData",     // Material data
            "textureData",      // Texture data
            "materialParams"    // Material parameters
        };
        outputs = {
            "processedMaterial",   // Processed material
            "materialStatus",   // Material processing status
            "materialMetadata"  // Material metadata
        }
    }
};

class MaterialProcessor : public AssetProcessor {
public:
    explicit MaterialProcessor(const MaterialProcessorInfo& info = MaterialProcessorInfo())
        : AssetProcessor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing === 
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MaterialProcessor() = default;     // Default destructor
};

} // namespace hd
