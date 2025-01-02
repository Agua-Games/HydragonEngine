/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Input management and device handling system
 */

#pragma once
#include "InputTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <functional>

namespace Hydragon::Input {

class InputSystem {
public:
    struct InputConfig {
        uint32_t maxDevices = 8;
        uint32_t maxBindings = 1000;
        uint32_t maxActions = 500;
        bool enableRawInput = true;
        bool enableHaptics = true;
        bool enableHotPlugging = true;
        float deadZone = 0.15f;
    };

    static InputSystem& Get();
    
    void Initialize(const InputConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterDevice(const DeviceDesc& desc);
    void UnregisterDevice(DeviceHandle handle);
    
    ActionHandle CreateAction(const ActionDesc& desc);
    void DestroyAction(ActionHandle handle);
    
    void BindAction(ActionHandle action, const ActionBinding& binding);
    void UnbindAction(ActionHandle action, const ActionBinding& binding);
    
    bool IsActionTriggered(ActionHandle action) const;
    float GetActionValue(ActionHandle action) const;
    
    void AddActionCallback(ActionHandle action, const ActionCallback& callback);
    void RemoveActionCallback(ActionHandle action, const ActionCallback& callback);
    
    void SetHapticFeedback(DeviceHandle device, const HapticEffect& effect);
    
    const InputStats& GetStats() const { return m_Stats; }

private:
    InputSystem() = default;
    
    InputConfig m_Config;
    InputStats m_Stats;
    std::unique_ptr<IInputProvider> m_Provider;
    std::vector<InputDevice> m_Devices;
    std::vector<InputAction> m_Actions;
    bool m_Initialized = false;
};

} // namespace Hydragon::Input 