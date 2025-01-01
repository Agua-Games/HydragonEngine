/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Default memory management strategy for Hydragon
 */

#pragma once
#include "Management/IMemoryStrategy.h"
#include <vector>

namespace Hydragon {
namespace Memory {

class DefaultMemoryStrategy : public IMemoryStrategy {
public:
    struct Config {
        size_t initialHeapSize = 1024 * 1024 * 64;  // 64MB
        bool enableDefrag = true;
        bool trackAllocations = true;
    };

    explicit DefaultMemoryStrategy(const Config& config = {});
    ~DefaultMemoryStrategy();

    // IMemoryStrategy interface
    void* allocate(size_t size, const AllocationInfo& info) override;
    void deallocate(void* ptr) override;
    bool owns(void* ptr) const override;
    MemoryStats getStats() const override;
    void compact() override;

private:
    struct HeapBlock {
        void* memory;
        size_t size;
        bool used;
        AllocationInfo info;
    };

    std::vector<HeapBlock> m_Blocks;
    void* m_HeapStart;
    size_t m_HeapSize;
    MemoryStats m_Stats;

    HeapBlock* findFreeBlock(size_t size);
    void splitBlock(HeapBlock* block, size_t size);
    void mergeAdjacentBlocks();
};

}} // namespace Hydragon::Memory 