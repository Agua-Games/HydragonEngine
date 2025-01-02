/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime event handling and message passing
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Core {

struct RuntimeEventConfig {
    bool enableEventBuffering = true;
    bool enablePriorityQueue = true;
    bool enableEventFiltering = true;
    bool enableEventLogging = true;
    uint32_t maxQueueSize = 1000;
    uint32_t maxHandlersPerEvent = 32;
    uint32_t bufferTimeMs = 16;  // 1 frame @ 60fps
};

class RuntimeEventSystem {
public:
    static RuntimeEventSystem& Get();
    
    void Initialize(const RuntimeEventConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterHandler(const std::string& event, const EventCallback& handler, EventPriority priority = EventPriority::Normal);
    void UnregisterHandler(const std::string& event, const EventCallback& handler);
    
    void PostEvent(const std::string& event, const EventData& data = {});
    void PostEventDeferred(const std::string& event, const EventData& data = {}, uint32_t delayMs = 0);
    
    void FlushEvents(const std::string& event = "");
    void CancelDeferredEvents(const std::string& event = "");
    
    const RuntimeEventStats& GetStats() const { return m_Stats; }

private:
    RuntimeEventSystem() = default;
    
    RuntimeEventConfig m_Config;
    RuntimeEventStats m_Stats;
    std::unordered_map<std::string, std::vector<EventHandlerEntry>> m_Handlers;
    std::vector<DeferredEvent> m_DeferredEvents;
    std::unique_ptr<IEventDispatcher> m_Dispatcher;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 