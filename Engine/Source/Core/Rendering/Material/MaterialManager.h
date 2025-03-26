/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * @file MaterialManager.h
 * @brief Header file for the MaterialManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - Support USD material binding protocols
 * - Enable efficient variant switching
 * - Maintain editor-time material networks
 * - Support runtime optimization of shader permutations
 * - Enable material instance pooling
 * - Support procedural material evolution
 */
#pragma once
#include "Node.h"
#include "Material.h"

namespace hd {

// Material variant configuration
struct VariantConfig {
    float performanceThreshold;     // When to switch variants
    uint32_t maxTextureResolution;  // Max texture size for this variant
    bool enableProcedural;          // Allow procedural features
    uint32_t shaderComplexityLevel; // Shader feature level
};

// Shared resource management
struct SharedResourceStrategy {
    enum class SharingMode {
        FULL_SHARE,          // Share all compatible resources
        SELECTIVE_SHARE,     // Share based on rules
        NO_SHARE            // Independent instances
    };

    struct SharingRules {
        bool shareTextures;      // Share texture resources
        bool shareParameters;    // Share parameter blocks
        bool sharePipelines;     // Share shader pipelines
    };
};

// Material data organization
struct StorageStrategy {
    enum class ParameterStorage {
        PACKED,             // Dense packed data
        SPARSE,            // Sparse parameter storage
        HIERARCHICAL       // Inheritance-based storage
    };

    // How variants store their differences from base
    enum class VariantStorage {
        FULL_COPY,         // Complete copy (memory heavy, fast)
        DELTA_ONLY,        // Store only differences (memory efficient)
        HYBRID            // Mix based on access patterns
    };
};
struct MaterialManagerInfo : public NodeInfo {
    VariantConfig variantConfig;
    SharedResourceStrategy sharedResourceStrategy;
    StorageStrategy storageStrategy;
    
    MaterialManagerInfo() {
        NodeType = "Rendering/MaterialManager";
        inputs = {
            "Materials",       // Array of materials
            "SceneGraph",      // Scene graph for material binding
            "PerformanceMetric"// Performance metric for LOD
        };
        outputs = {
            "ActiveMaterials", // Active material instances
            "LODLevels",       // LOD levels for each material
            "ResourceUsage"    // Resource usage statistics
        };
    }
};

class MaterialManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MaterialManager(const MaterialManagerInfo& info = MaterialManagerInfo())
        : Node(info), MaterialManagerInfo(info) {}
    void initialize();

    // === Processing ===
    void () {
        update();
    }
    void update();
    void addMaterial(const std::shared_ptr<Material>& material);
    void removeMaterial(const std::shared_ptr<Material>& material);

    // === Cleanup ===
    ~MaterialManager() = default;
private:
    std::vector<std::shared_ptr<Material>> materials;
};

} // namespace hd