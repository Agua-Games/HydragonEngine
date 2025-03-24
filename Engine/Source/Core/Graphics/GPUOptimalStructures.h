/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * This is still a sketch, on how to design the data structures and interfaces to allow for GPU optimization - for performance gains
 *  while keeping quality. This file will change a lot, probably becoming one or more classes.
 * 
 * ARCHITECTURAL NOTES:
 * - Rules should be data-driven, loaded from config
 * - Support runtime format switching based on distance
 * - Enable easy debugging/visualization of compression
 * - Maintain original data for editor/tools
 * 
 * TODO:
 *  - Check if the forward declaration of the structs (suggested by the assistant) matches the intended use.
 */
#if 0
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace hd {

    // Forward declarations for the GPUOptimalStructures namespace
    struct GPUVertexBatch;
    struct GPUFriendlyMesh;
    struct RenderBatch;
    struct GPUMemoryLayout;
    struct ComputeWorkgroup;

// === Structure Definitions ===
// 1. Align with GPU's SIMD width (usually 32 elements)
struct GPUVertexBatch {
    alignas(16) float positions[32 * 3];    // 32 vertices, xyz
    alignas(16) float normals[32 * 3];      // 32 normals, xyz
    uint32_t count;                         // Active elements
};

// 2. Match GPU's preferred memory patterns
struct GPUFriendlyMesh {
    // SoA (Structure of Arrays) - Better for GPU SIMD
    std::vector<float> allPositionsX;       // All X coordinates together
    std::vector<float> allPositionsY;       // All Y coordinates together
    std::vector<float> allPositionsZ;       // All Z coordinates together
    
    // Instead of AoS (Array of Structures):
    // struct Vertex { float x, y, z; };
    // std::vector<Vertex> vertices;        // Worse for GPU processing
};

// 3. Batch-oriented command structure
struct RenderBatch {
    static constexpr uint32_t OPTIMAL_BATCH_SIZE = 256; // Typical GPU wavefront size
    
    // Packed data for efficient upload
    alignas(16) glm::mat4 transforms[OPTIMAL_BATCH_SIZE];
    alignas(16) uint32_t materialIndices[OPTIMAL_BATCH_SIZE];
    uint32_t instanceCount;

    // Matches Vulkan's info struct pattern
    VkDescriptorBufferInfo bufferInfo;
    VkWriteDescriptorSet descriptorWrite;
};

// 4. GPU-friendly memory layout
struct GPUMemoryLayout {
    enum class AccessPattern {
        SEQUENTIAL_READ,      // Best for vertex data
        RANDOM_ACCESS,        // For textures/resources
        COHERENT_READ_WRITE  // For compute shaders
    };

    struct BufferLayout {
        VkDeviceSize size;
        VkDeviceSize alignment;   // Usually 256 bytes for optimal access
        AccessPattern pattern;
        bool isCoherent;         // Affects memory type selection
    };
};

// 5. Compute-oriented structures
struct ComputeWorkgroup {
    static constexpr uint32_t WORKGROUP_SIZE = 256;  // Typical GPU wavefront
    
    struct Dispatch {
        uint32_t x, y, z;        // Multiples of WORKGROUP_SIZE
        bool enableAsyncCompute;  // Use async compute queue
    };

    // Packed data for compute
    alignas(16) float data[WORKGROUP_SIZE];
    uint32_t elementCount;
};

} // namespace hd

#endif