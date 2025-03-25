/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TextureProcessor.h
 * @brief Header file for the TextureProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - TextureProcessor is a class that represents a texture processor in Hydragon.
 * - It is used to process textures and their associated data, such as image data, texture parameters, etc.
 * - It uses the Vulkan API for texture processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Texture.h"

namespace hd {

struct TextureProcessorInfo : public AssetProcessorInfo {
    TextureProcessorInfo() {
        NodeType = "Core/TextureProcessor";
        inputs = {
            "TextureData",     // Texture data
            "TextureParams",   // Texture parameters
            "ImageMetadata"    // Image metadata
        };
        outputs = {
            "ProcessedTexture",   // Processed texture
            "TextureStatus",   // Texture processing status
            "TextureMetadata"  // Texture metadata
        }
    }
};

class TextureProcessor : public AssetProcessor {
public:
    explicit TextureProcessor(const TextureProcessorInfo& info = TextureProcessorInfo())
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
    ~TextureProcessor() = default;     // Default destructor
};

} // namespace hd
