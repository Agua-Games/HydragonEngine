/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SceneAllocator.h
 * @brief Header file for the SceneAllocator class.
 * 
 * ARCHITECTURAL NOTES:
 * - SceneAllocator is a singleton class that manages the allocation of scene data.
 * - It is responsible for allocating and deallocating scene data.
 * - It uses the Vulkan Memory Allocator (VMA) for GPU memory management.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

#include <vma/vk_mem_alloc.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace hd {

/**
 * @brief Allocator for scene data
 */
template<typename T>
class PoolAllocator {
public:
    explicit PoolAllocator(size_t initialSize = 1024) {
        pool.reserve(initialSize);
    }

    T* allocate() {
        if (freeList.empty()) {
            pool.emplace_back();
            return &pool.back();
        }
        size_t index = freeList.back();
        freeList.pop_back();
        return &pool[index];
    }

    void deallocate(T* ptr) {
        size_t index = ptr - pool.data();
        freeList.push_back(index);
    }

private:
    std::vector<T> pool;
    std::vector<size_t> freeList;
};

struct MaterialData {
    uint32_t albedoTextureIndex;
    uint32_t normalTextureIndex;
    uint32_t metallicRoughnessTextureIndex;
    glm::vec4 baseColor;
    float metallic;
    float roughness;
    // Add other material properties as needed
};

class SceneAllocator {
public:
    static constexpr size_t LARGE_ALLOCATION_THRESHOLD = 1024 * 1024;  // 1MB

    struct AllocationStrategy {
        size_t transformPoolSize = 1024;
        size_t geometryPoolSize = 1024;
        size_t materialPoolSize = 256;
        bool useVMAForGeometry = true;
    };

    explicit SceneAllocator(const AllocationStrategy& strategy = AllocationStrategy(),
                             VmaAllocator vmaAllocator = nullptr)
        : transformPool(strategy.transformPoolSize),
          materialPool(strategy.materialPoolSize),
          vmaAllocator(vmaAllocator) {}

    struct GeometryAllocation {
        void* cpuData;
        VmaAllocation gpuAllocation;
        VkBuffer buffer;
        size_t size;
    };

    GeometryAllocation allocateGeometry(size_t size) {
        if (size > LARGE_ALLOCATION_THRESHOLD) {
            return allocateViaVMA(size);
        }
        return allocateFromPool(size);
    }

    void deallocateGeometry(GeometryAllocation& allocation) {
        if (allocation.gpuAllocation) {
            vmaDestroyBuffer(vmaAllocator, allocation.buffer, allocation.gpuAllocation);
        } else {
            geometryPool.deallocate(static_cast<uint8_t*>(allocation.cpuData));
        }
        allocation = GeometryAllocation{};
    }

    glm::mat4* allocateTransform() {
        return transformPool.allocate();
    }

    MaterialData* allocateMaterial() {
        return materialPool.allocate();
    }

    void deallocateTransform(glm::mat4* ptr) {
        transformPool.deallocate(ptr);
    }

    void deallocateMaterial(MaterialData* ptr) {
        materialPool.deallocate(ptr);
    }

private:
    PoolAllocator<glm::mat4> transformPool;
    PoolAllocator<MaterialData> materialPool;
    PoolAllocator<uint8_t> geometryPool;
    VmaAllocator vmaAllocator;

    GeometryAllocation allocateViaVMA(size_t size) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        GeometryAllocation allocation = {};
        allocation.size = size;

        VkBuffer buffer;
        VmaAllocation vmaAllocation;
        if (vmaCreateBuffer(vmaAllocator, &bufferInfo, &allocInfo, &buffer,
                          &vmaAllocation, nullptr) == VK_SUCCESS) {
            allocation.buffer = buffer;
            allocation.gpuAllocation = vmaAllocation;
        }

        return allocation;
    }

    GeometryAllocation allocateFromPool(size_t size) {
        GeometryAllocation allocation = {};
        allocation.cpuData = geometryPool.allocate();
        allocation.size = size;
        return allocation;
    }
};

} // namespace hd