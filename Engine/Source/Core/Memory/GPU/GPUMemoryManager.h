#pragma once
#include "../Management/MemoryManager.h"
#include <vulkan/vulkan.h>  // Example, can be made API-agnostic

namespace Hydragon {
namespace Memory {

class GPUMemoryManager {
public:
    struct GPUAllocation {
        VkDeviceMemory memory;
        VkDeviceSize size;
        VkDeviceSize offset;
        uint32_t memoryType;
    };
    
    GPUAllocation AllocateGPUMemory(VkMemoryRequirements reqs, 
                                   VkMemoryPropertyFlags props);
    void DeallocateGPUMemory(GPUAllocation& allocation);
    
    // GPU memory tracking
    size_t GetTotalGPUMemoryUsed() const;
    void DumpGPUMemoryStats() const;

private:
    struct GPUMemoryHeap {
        VkDeviceMemory memory;
        size_t size;
        size_t used;
    };
    std::vector<GPUMemoryHeap> m_Heaps;
};

}} // namespace Hydragon::Memory 