/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MaterialAtlas.h
 * @brief MaterialAtlas represents a material atlas node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Treats PBR texture sets as single, packable assets
 * - Uses Vulkan-inspired descriptor layouts for channel mapping
 * - Supports dynamic reinterpretation through image views
 * - Enables node-based transformations and caching
 * - Integrates with TiledAtlasTexturing system
 */
#pragma once
#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "TiledAtlasTexturing.h"
#include "Node.h"

namespace hd {

struct MaterialAtlasInfo : public NodeInfo {
    MaterialAtlasInfo() {
        NodeType = "Graphics/MaterialAtlas";
        
        Inputs = {
            "MaterialSets",      // Array of material texture sets
            "AtlasConfig",       // Atlas configuration
            "StreamingConfig",   // Streaming parameters
            "ProcessingStage"    // Current processing stage
        };
        
        Outputs = {
            "AtlasTexture",      // Packed texture atlas
            "DescriptorSet",     // Vulkan descriptor set
            "MaterialIndices",   // Material lookup indices
            "CacheState"         // Processing cache state
        };
    }
};

class MaterialAtlas : public Node {
public:
    // === Structure Definitions ===
    // Describes how channels are packed/interpreted
    struct AtlasLayout {
        enum class ChannelPacking {
            RGBA_STANDARD,     // Traditional separate RGBA
            OCTAHEDRON_NORMAL, // Compressed normal encoding
            PACKED_PBR,        // Metallic(R) Roughness(G) AO(B)
            CUSTOM_SWIZZLE     // User-defined channel mapping
        };

        struct ChannelMapping {
            VkFormat format;
            VkImageViewType viewType;
            VkComponentMapping swizzle;
            ChannelPacking packing;
        };
    };

    // PBR texture set as a single asset
    struct MaterialSet {
        struct TextureLayer {
            uint32_t baseLayer;           // Base layer in atlas
            uint32_t mipCount;            // Number of mip levels
            VkFormat format;              // Layer format
            bool compressed;              // Using hardware compression
        };

        // Standard PBR maps
        TextureLayer albedo;              // Base color (sRGB)
        TextureLayer normal;              // Normal map (compressed)
        TextureLayer metallicRoughness;   // Packed metal/rough
        TextureLayer occlusion;           // Ambient occlusion
        TextureLayer emission;            // Emissive (optional)
        
        // Metadata for processing
        std::string materialId;           // Unique identifier
        uint32_t atlasIndex;             // Index in material atlas
        bool isDynamic;                   // Allows runtime updates
    };

    // Atlas management
    struct AtlasPool {
        VkImage atlasImage;              // Texture array for sets
        VkImageView atlasView;           // View for binding
        VkDescriptorSetLayout layout;    // Descriptor layout
        uint32_t maxSets;                // Maximum material sets
    };

    // Memory and streaming configuration
    struct StreamingConfig {
        bool enableCompression;         // Use hardware compression
        bool allowDynamicUpdates;       // Runtime updates
        uint32_t maxConcurrentLoads;    // Concurrent transfers
        size_t memoryBudget;            // Memory limit
    };

    struct CacheState {
        bool isValid;
        uint64_t lastUpdateTime;
        std::unordered_map<std::string, uint64_t> materialHashes;
    } cacheState;

    // === Allocation, Initialization, Loading ===
    explicit MaterialAtlas(const MaterialAtlasInfo& info = MaterialAtlasInfo())
        : Node(info), AtlasInfo(info) {}

    MaterialSet createMaterialSet(const std::string& materialId);

    // === Atlas management ===
    VkDescriptorSet bindMaterialSet(const MaterialSet& set);
    void unbindMaterialSet(const MaterialSet& set);

    // === Procedural Generation ===
    void evolve() override;        // Time-based evolution
    void adapt() override;         // Context adaptation
    void selfModify() override;    // Behavior modification
    void harmonize() override;     // System balance

    // === Caching & Optimization ===
    void optimizeAtlas();
    void invalidateCache(const std::string& materialId);

    // === Processing ===
    void processNode() override {
        auto materialSets = getInputValue<std::vector<MaterialSet>>("MaterialSets");
        auto config = getInputValue<StreamingConfig>("StreamingConfig");
        
        if (isDirty || !tryUseCache()) {
            processAtlas(materialSets, config);
            updateCache();
        }

        setOutputValue("AtlasTexture", atlasPool.atlasImage);
        setOutputValue("DescriptorSet", currentDescriptorSet);
        setOutputValue("MaterialIndices", materialIndices);
        setOutputValue("CacheState", cacheState);
    }

    void updateMaterialSet(const MaterialSet& set);

protected:
    // === Caching & Optimization ===
    uint64_t computeCacheKey() const override;

    // === Compilation ===
    void generateRuntimeCode(CodeGenContext& context) override;

private:
    // === Allocation, Initialization, Loading ===
    MaterialAtlasInfo AtlasInfo;
    AtlasPool atlasPool;
    VkDescriptorSet currentDescriptorSet;
    std::vector<uint32_t> materialIndices;
    
    VkResult initializeAtlas(VkDevice device, const StreamingConfig& config);

    // === Processing ===
    void processAtlas(const std::vector<MaterialSet>& sets, const StreamingConfig& config);
    void updateCache();
};

} // namespace hd