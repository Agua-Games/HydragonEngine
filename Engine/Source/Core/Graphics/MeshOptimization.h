/**
 * This is still a sketch, on how to design the data structures and interfaces to allow for mesh optimization - for performance gains while
 * keeping quality. This file will change a lot, probably becoming one or more classes.
 * 
 * ARCHITECTURAL NOTES:
 * - Rules should be data-driven, loaded from config
 * - Support runtime format switching based on distance
 * - Enable easy debugging/visualization of compression
 * - Maintain original data for editor/tools
 * 
 */
#pragma once

namespace hd {

struct MeshOptimizationStrategy {
    enum class VertexFormat {
        FULL_PRECISION,      // 32-bit (for editor/tools)
        HALF_PRECISION,      // 16-bit (most game content)
        QUARTER_PRECISION,   // 8-bit (far distance LODs)
        CUSTOM              // For special cases
    };

    enum class NormalEncoding {
        FULL_XYZ,           // Debug/Editor
        OCTAHEDRON,         // Main game usage
        SPHEREMAP,          // Alternative for specific needs
        CUSTOM             // For special cases
    };

    struct OptimizationRules {
        // Memory vs Quality thresholds
        float qualityThreshold;    // 0-1, guides compression decisions
        float distanceThreshold;   // When to switch formats
        bool allowVertexReuse;     // Share vertices between LODs
        bool enableQuantization;   // Enable position quantization
    };

    // Distance-based LOD system
    struct AdvancedLODSystem {
        /** Hybrid mesh/impostor system. 
        * The generated impostor (can be a sheet or a cubemap), when created from rendering many meshes together, could also be thought of as a "3d matte painting", 
        * or matte rendering, effectively working as a set extension matte painting which looks like a 3D scene. For a seamless look, we must use:
        *  - Alpha and/or depth blending for the transition zone
        *  - Maybe also dithering
        *  - Render layer/pass tags for moving objects, to be excluded from the render target based matte rendering
        *  - Some helper passes needed to fake dynamic lighting changes, etc, like a world normal attachment/pass, maybe flow fields, etc.
        *  - For distances that won't be ever reached by the player we can also resort to Renderer generated ai-assisted images, (details) enhancement, which
        * start from the matte rendering as the latent image used, passed to the sampler.
        * */
        struct HybridRenderer {
            float switchDistance;
            std::shared_ptr<Mesh> meshLOD;
            std::shared_ptr<Impostor> impostor;
        };

        // Object merging for distant objects.
        struct MergedInstance {
            AABB bounds;
            std::vector<Matrix4x4> instances;
            std::shared_ptr<Mesh> mergedMesh;
        };
    };
};

} // namespace hd