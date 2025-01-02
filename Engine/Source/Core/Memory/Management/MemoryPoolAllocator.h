/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Pool-based memory allocator for fixed-size allocations
 */

#pragma once
#include "IMemoryStrategy.h"
#include "Core/Memory/Types/MemoryTypes.h"

namespace Hydragon::Memory {

class MemoryPoolAllocator : public IMemoryStrategy {
public:
    struct PoolConfig {
        size_t blockSize;           ///< Size of each block in the pool
        size_t initialBlockCount;   ///< Initial number of blocks to allocate
        size_t maxBlockCount;       ///< Maximum number of blocks allowed
        bool allowGrowth = true;    ///< Whether pool can grow beyond initial size
    };

    explicit MemoryPoolAllocator(const PoolConfig& config);
    ~MemoryPoolAllocator() override;

    void* allocate(size_t size, const AllocationInfo& info) override;
    void deallocate(void* ptr) override;
    
    // Pool statistics
    size_t getFreeBlockCount() const;
    size_t getTotalBlockCount() const;
    float getFragmentation() const;

private:
    PoolConfig m_Config;
    struct BlockHeader;
    BlockHeader* m_FreeList = nullptr;
    size_t m_AllocatedBlocks = 0;
};

} // namespace Hydragon::Memory 