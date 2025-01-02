/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Audio streaming system
 */

#pragma once
#include "../AudioTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <queue>

namespace Hydragon::Audio {

class AudioStreamingSystem {
public:
    struct StreamingConfig {
        size_t bufferSize = 256 * 1024;  // 256KB per stream
        uint32_t maxConcurrentStreams = 8;
        uint32_t prebufferCount = 3;
        float bufferUpdateInterval = 0.1f;  // 100ms
    };

    void Initialize(const StreamingConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void StartStreaming(SoundHandle handle, const StreamParams& params = {});
    void StopStreaming(SoundHandle handle);
    
    bool IsStreaming(SoundHandle handle) const;
    float GetBufferProgress(SoundHandle handle) const;
    
    const StreamingStats& GetStats() const { return m_Stats; }

private:
    StreamingConfig m_Config;
    StreamingStats m_Stats;
    std::unique_ptr<IStreamingProvider> m_Provider;
    std::queue<StreamRequest> m_PendingRequests;
    bool m_Initialized = false;
};

} // namespace Hydragon::Audio 