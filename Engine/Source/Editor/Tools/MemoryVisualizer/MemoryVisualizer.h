/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory visualization and debugging utilities for the editor
 */

#pragma once
#include "Core/Memory/MemorySystem.h"
#include "Core/Memory/MemoryVisualization.h"
#include "Core/Memory/Debug/MemoryDebug.h"
#include "Editor/Core/EditorTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Editor::Tools {

class MemoryVisualizer {
public:
    static MemoryVisualizer& Get();
    
    void Initialize(const MemoryVisualizerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Real-time memory usage visualization
    struct VisualizationData {
        struct Region {
            const char* name;
            size_t size;
            float fragmentation;
            std::vector<Memory::AllocationEvent> recentEvents;
        };
        std::vector<Region> regions;
    };

    // Live memory tracking for editor
    void TrackAllocation(const Memory::AllocationInfo& info);
    void GenerateHeatMap();
    void ExportVisualization(const std::string& path);
    
    const MemoryVisualizerStats& GetStats() const { return m_Stats; }

private:
    MemoryVisualizer() = default;
    void UpdateVisualization(const Memory::AllocationInfo& info);
    
    MemoryVisualizerConfig m_Config;
    MemoryVisualizerStats m_Stats;
    std::unique_ptr<Memory::IMemoryAnalyzer> m_Analyzer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor::Tools 