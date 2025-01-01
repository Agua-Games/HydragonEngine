/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Streaming allocator for Hydragon
 */

#pragma once
#include "../IMemoryStrategy.h"
#include <queue>

namespace Hydragon {
namespace Memory {

class StreamingAllocator : public IMemoryStrategy {
public:
    struct StreamConfig {
        size_t streamBufferSize;
        size_t minStreamBlockSize;
        bool enableDefrag;
    };

    explicit StreamingAllocator(const StreamConfig& config);
    
    void* Allocate(size_t size, size_t alignment) override;
    void Deallocate(void* ptr) override;
    
    // Streaming features
    void BeginFrame();
    void EndFrame();
    void FlushStream();

private:
    struct StreamBlock {
        void* ptr;
        size_t size;
        uint64_t frameId;
    };
    
    std::queue<StreamBlock> m_PendingDeallocs;
    uint64_t m_CurrentFrame = 0;
    StreamConfig m_Config;
};

}} // namespace Hydragon::Memory 