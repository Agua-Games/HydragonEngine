/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime state management and transitions
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeStateConfig {
    bool enableStateHistory = true;
    bool enableTransitionBlending = true;
    bool enableStateValidation = true;
    bool enableStateRecovery = true;
    uint32_t maxHistoryEntries = 100;
    float blendDuration = 0.3f;
    std::string defaultState = "Idle";
};

class RuntimeStateSystem {
public:
    static RuntimeStateSystem& Get();
    
    void Initialize(const RuntimeStateConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterState(const std::string& name, std::unique_ptr<IState> state);
    void UnregisterState(const std::string& name);
    
    void TransitionTo(const std::string& state, const TransitionDesc& desc = {});
    void PushState(const std::string& state);
    void PopState();
    
    const std::string& GetCurrentState() const;
    bool IsInTransition() const;
    
    const RuntimeStateStats& GetStats() const { return m_Stats; }

private:
    RuntimeStateSystem() = default;
    
    RuntimeStateConfig m_Config;
    RuntimeStateStats m_Stats;
    std::vector<StateInstance> m_StateStack;
    std::unordered_map<std::string, std::unique_ptr<IState>> m_States;
    std::unique_ptr<IStateManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 