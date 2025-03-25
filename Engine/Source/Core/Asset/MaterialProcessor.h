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
        NodeType = "Core/MaterialProcessor";
        inputs = {
            "MaterialData",     // Material data
            "TextureData",      // Texture data
            "MaterialParams"    // Material parameters
        };
        outputs = {
            "ProcessedMaterial",   // Processed material
            "MaterialStatus",   // Material processing status
            "MaterialMetadata"  // Material metadata
        }
    }
};

class MaterialProcessor : public AssetProcessor {
public:
    explicit MaterialProcessor(const MaterialProcessorInfo& info = MaterialProcessorInfo())
        : AssetProcessor(info) {}
    initialize() override {}
    load() override {}

    // === Processing === 
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MaterialProcessor() = default;     // Default destructor
};

} // namespace hd
