/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset streaming system
 */

#pragma once
#include "../AssetTypes.h"
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"
#include <queue>

namespace Hydragon::Assets {

class AssetStreamingSystem {
public:
    struct StreamingConfig {
        size_t streamingBudget = 1024 * 1024 * 1024;  // 1GB
        uint32_t maxConcurrentStreams = 8;
        bool enablePrioritization = true;
        float streamingLatencyMs = 16.67f;  // Target 60 FPS
    };

    void Initialize(const StreamingConfig& config = {});
    void Shutdown();

    void RequestStream(AssetHandle handle, const LoadParams& params);
    void CancelStream(AssetHandle handle);
    
    void Update();  // Called each frame
    bool IsStreaming(AssetHandle handle) const;

private:
    struct StreamRequest {
        AssetHandle handle;
        LoadParams params;
        float priority;
    };

    StreamingConfig m_Config;
    std::priority_queue<StreamRequest> m_PendingRequests;
    std::unordered_map<AssetHandle, float> m_StreamingProgress;
};

} // namespace Hydragon::Assets 