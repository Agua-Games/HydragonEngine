#pragma once
#include "IMemoryStrategy.h"
#include "Core/Assets/StreamingController.h"

namespace Hydragon {
namespace Memory {

class StreamingMemoryStrategy : public IMemoryStrategy {
public:
    struct StreamingConfig {
        size_t streamingPoolSize = 256 * 1024 * 1024;  // 256MB
        size_t maxStreamingRequests = 64;
        bool enablePrioritization = true;
    };

    void* allocateStreamingMemory(const Assets::StreamRequest& request) {
        // Integrate with memory pressure monitoring
        if (m_PressureMonitor.isUnderPressure()) {
            evictLowPriorityAssets();
        }

        // Allocate with streaming hints
        return allocateWithHints({
            .isStreamable = true,
            .priority = request.priority,
            .expectedLifetime = request.expectedLifetime
        });
    }

private:
    struct StreamingBlock {
        void* memory;
        size_t size;
        float priority;
        uint64_t lastAccess;
    };

    // Track streaming memory usage for better eviction
    std::vector<StreamingBlock> m_StreamingBlocks;
};

}} // namespace Hydragon::Memory 