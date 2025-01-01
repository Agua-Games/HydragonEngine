#pragma once
#include "Core/Memory/MemorySystem.h"

namespace Hydragon {
namespace Debug {

class MemoryVisualizer {
public:
    // Real-time memory usage visualization
    struct VisualizationData {
        struct Region {
            const char* name;
            size_t size;
            float fragmentation;
            std::vector<AllocationEvent> recentEvents;
        };
        std::vector<Region> regions;
    };

    // Live memory tracking for editor
    void trackAllocation(const Memory::AllocationInfo& info) {
        if (m_Config.enableLiveTracking) {
            updateVisualization(info);
        }
    }

    // Memory heat map generation
    void generateHeatMap() {
        // Show memory access patterns
        // Useful for optimizing node graph execution
    }
};

}} // namespace Hydragon::Debug 