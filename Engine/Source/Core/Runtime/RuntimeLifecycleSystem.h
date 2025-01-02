/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime lifecycle and application state management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Core {

struct RuntimeLifecycleConfig {
    bool enableGracefulShutdown = true;
    bool enableStateRecovery = true;
    bool enableCrashHandling = true;
    bool enableSessionTracking = true;
    uint32_t shutdownTimeoutMs = 5000;
    uint32_t recoveryAttempts = 3;
    std::string crashDumpPath = "Logs/Crashes";
};

class RuntimeLifecycleSystem {
public:
    static RuntimeLifecycleSystem& Get();
    
    void Initialize(const RuntimeLifecycleConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterLifecycleHandler(const std::string& phase, const LifecycleCallback& callback);
    void UnregisterLifecycleHandler(const std::string& phase);
    
    void RequestShutdown(ShutdownReason reason = ShutdownReason::User);
    void CancelShutdown();
    
    void SaveApplicationState();
    void RestoreApplicationState();
    
    LifecycleState GetCurrentState() const;
    const RuntimeLifecycleStats& GetStats() const { return m_Stats; }

private:
    RuntimeLifecycleSystem() = default;
    
    RuntimeLifecycleConfig m_Config;
    RuntimeLifecycleStats m_Stats;
    std::unordered_map<std::string, LifecycleCallback> m_Handlers;
    std::unique_ptr<IStateSerializer> m_Serializer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 