/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#include "Editor/Tools/MemoryVisualizer/MemoryVisualizer.h"
#include "Core/Memory/MemorySystem.h"
#include "Core/Debug/Logging.h"

namespace Hydragon::Editor::Tools {

MemoryVisualizer& MemoryVisualizer::Get() {
    static MemoryVisualizer instance;
    return instance;
}

void MemoryVisualizer::Initialize(const MemoryVisualizerConfig& config) {
    if (m_Initialized) {
        LOG_WARNING("MemoryVisualizer already initialized");
        return;
    }
    
    m_Config = config;
    m_Analyzer = std::make_unique<Memory::MemoryAnalyzer>();
    m_Initialized = true;
    
    LOG_INFO("MemoryVisualizer initialized");
}

void MemoryVisualizer::TrackAllocation(const Memory::AllocationInfo& info) {
    if (!m_Initialized || !m_Config.enableLiveTracking) {
        return;
    }
    UpdateVisualization(info);
}

// ... rest of implementation

} // namespace Hydragon::Editor::Tools 