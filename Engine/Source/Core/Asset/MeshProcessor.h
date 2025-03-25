/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MeshProcessor.h
 * @brief Header file for the MeshProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - MeshProcessor is a class that represents a mesh processor in Hydragon.
 * - It is used to process meshes and their associated data, such as vertex data, index data, material data, etc.
 * - It uses the Vulkan API for mesh processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Mesh.h"

namespace hd {

struct MeshProcessorInfo : public AssetProcessorInfo {
    MeshProcessorInfo() {
        NodeType = "Core/MeshProcessor";
        inputs = {
            "MeshData",     // Mesh data
            "MaterialData", // Material data
            "TextureData"   // Texture data
        };
        outputs = {
            "ProcessedMesh",   // Processed mesh
            "MeshStatus",   // Mesh processing status
            "MeshMetadata"  // Mesh metadata
        }
    }
};

class MeshProcessor : public AssetProcessor {
    public:
        // === Allocation, Initialization, Loading ===
        explicit MeshProcessor(const MeshProcessorInfo& info = MeshProcessorInfo())
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
        ~MeshProcessor() = default;     // Default destructor
};

} // namespace hd
