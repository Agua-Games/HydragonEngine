/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Profiler backend implementation
 */

#pragma once
#include "../ProfilerTypes.h"
#include <memory>
#include <string>

namespace Hydragon::Profiling {

class IProfilerBackend {
public:
    virtual ~IProfilerBackend() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    
    virtual void PushEvent(const ProfilerEvent& event) = 0;
    virtual void PopEvent() = 0;
    
    virtual void BeginGPUScope(const char* name) = 0;
    virtual void EndGPUScope() = 0;
    
    virtual void ExportCapture(const std::string& path) = 0;
    virtual void ClearCapture() = 0;
};

class ChromeTracingBackend : public IProfilerBackend {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void BeginFrame() override;
    void EndFrame() override;
    
    void PushEvent(const ProfilerEvent& event) override;
    void PopEvent() override;
    
    void BeginGPUScope(const char* name) override;
    void EndGPUScope() override;
    
    void ExportCapture(const std::string& path) override;
    void ClearCapture() override;

private:
    std::vector<ProfilerEvent> m_EventStack;
    std::vector<ProfilerFrame> m_Frames;
};

} // namespace Hydragon::Profiling 