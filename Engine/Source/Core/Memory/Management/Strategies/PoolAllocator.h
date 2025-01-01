#pragma once
#include "../IMemoryStrategy.h"
#include <vector>
#include <array>

namespace Hydragon {
namespace Memory {

class PoolAllocator : public IMemoryStrategy {
public:
    struct PoolConfig {
        size_t blockSize;
        size_t blocksPerPool;
        bool growOnDemand;
    };

    explicit PoolAllocator(const PoolConfig& config);
    
    void* Allocate(size_t size, size_t alignment) override;
    void Deallocate(void* ptr) override;
    
    // Pool management
    void Defragment();
    size_t GetFreeBlocks() const;

private:
    struct MemoryPool {
        void* memory;
        std::vector<bool> blockUsage;
        size_t freeBlocks;
    };
    
    std::vector<MemoryPool> m_Pools;
    PoolConfig m_Config;
    
    MemoryPool* CreatePool();
};

}} // namespace Hydragon::Memory 