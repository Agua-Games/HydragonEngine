/**
 * This is still a sketch on how to design the material system.
 * This file is expected to change a lot, incorporate new designs and ideas.
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

namespace hd {

struct MaterialSystemStrategy {
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
};

} // namespace hd