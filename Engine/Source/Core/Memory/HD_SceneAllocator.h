/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
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

    T* Allocate() {
        if (freeList.empty()) {
            pool.emplace_back();
            return &pool.back();
        }
        size_t index = freeList.back();
        freeList.pop_back();
        return &pool[index];
    }

    void Deallocate(T* ptr) {
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

class HD_SceneAllocator {
public:
    static constexpr size_t LARGE_ALLOCATION_THRESHOLD = 1024 * 1024;  // 1MB

    struct AllocationStrategy {
        size_t transformPoolSize = 1024;
        size_t geometryPoolSize = 1024;
        size_t materialPoolSize = 256;
        bool useVMAForGeometry = true;
    };

    explicit HD_SceneAllocator(const AllocationStrategy& strategy = AllocationStrategy(),
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

    GeometryAllocation AllocateGeometry(size_t size) {
        if (size > LARGE_ALLOCATION_THRESHOLD) {
            return AllocateViaVMA(size);
        }
        return AllocateFromPool(size);
    }

    void DeallocateGeometry(GeometryAllocation& allocation) {
        if (allocation.gpuAllocation) {
            vmaDestroyBuffer(vmaAllocator, allocation.buffer, allocation.gpuAllocation);
        } else {
            geometryPool.Deallocate(static_cast<uint8_t*>(allocation.cpuData));
        }
        allocation = GeometryAllocation{};
    }

    glm::mat4* AllocateTransform() {
        return transformPool.Allocate();
    }

    MaterialData* AllocateMaterial() {
        return materialPool.Allocate();
    }

    void DeallocateTransform(glm::mat4* ptr) {
        transformPool.Deallocate(ptr);
    }

    void DeallocateMaterial(MaterialData* ptr) {
        materialPool.Deallocate(ptr);
    }

private:
    PoolAllocator<glm::mat4> transformPool;
    PoolAllocator<MaterialData> materialPool;
    PoolAllocator<uint8_t> geometryPool;
    VmaAllocator vmaAllocator;

    GeometryAllocation AllocateViaVMA(size_t size) {
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

    GeometryAllocation AllocateFromPool(size_t size) {
        GeometryAllocation allocation = {};
        allocation.cpuData = geometryPool.Allocate();
        allocation.size = size;
        return allocation;
    }
};

} // namespace hd