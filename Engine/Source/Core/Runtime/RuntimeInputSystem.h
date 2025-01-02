/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime input processing and device management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeInputConfig {
    bool enableInputBuffering = true;
    bool enableDeviceHotplug = true;
    bool enableInputRecording = true;
    bool enableHapticFeedback = true;
    uint32_t maxInputDevices = 8;
    uint32_t inputBufferSize = 64;
    float deadZoneThreshold = 0.15f;
    std::string inputMappingFile = "Config/InputMapping.json";
};

class RuntimeInputSystem {
public:
    static RuntimeInputSystem& Get();
    
    void Initialize(const RuntimeInputConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterInputMap(const std::string& name, const InputMapDesc& desc);
    void UnregisterInputMap(const std::string& name);
    
    void SetActiveInputMap(const std::string& name);
    void EnableInputContext(const std::string& context);
    void DisableInputContext(const std::string& context);
    
    void ProcessInputEvent(const InputEvent& event);
    bool IsActionPressed(const std::string& action) const;
    float GetAxisValue(const std::string& axis) const;
    
    void StartInputRecording(const std::string& filename);
    void StopInputRecording();
    void PlaybackRecording(const std::string& filename);
    
    const RuntimeInputStats& GetStats() const { return m_Stats; }

private:
    RuntimeInputSystem() = default;
    
    RuntimeInputConfig m_Config;
    RuntimeInputStats m_Stats;
    std::unordered_map<std::string, InputMapInstance> m_InputMaps;
    std::vector<InputDeviceState> m_DeviceStates;
    std::unique_ptr<IInputProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 