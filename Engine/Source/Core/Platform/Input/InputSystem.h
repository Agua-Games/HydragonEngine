/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Input handling and device management
 */

#pragma once
#include "../PlatformTypes.h"
#include <memory>
#include <vector>
#include <functional>

namespace Hydragon::Platform {

class InputSystem {
public:
    struct InputConfig {
        bool enableRawInput = true;
        bool enableGamepad = true;
        bool enableMultitouch = true;
        bool enableHaptics = true;
        uint32_t maxGamepads = 4;
        uint32_t maxTouchPoints = 10;
    };

    static InputSystem& Get();
    
    void Initialize(const InputConfig& config = {});
    void Shutdown();
    
    void Update();
    
    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyReleased(KeyCode key) const;
    bool IsKeyHeld(KeyCode key) const;
    
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;
    Vector2 GetMousePosition() const;
    Vector2 GetMouseDelta() const;
    
    bool IsGamepadConnected(uint32_t index) const;
    float GetGamepadAxis(uint32_t index, GamepadAxis axis) const;
    bool IsGamepadButtonPressed(uint32_t index, GamepadButton button) const;
    
    void SetMouseMode(MouseMode mode);
    void SetGamepadVibration(uint32_t index, float leftMotor, float rightMotor);
    
    void RegisterInputCallback(const InputCallback& callback);
    void UnregisterInputCallback(InputCallbackId id);
    
    const InputStats& GetStats() const { return m_Stats; }

private:
    InputSystem() = default;
    
    InputConfig m_Config;
    InputStats m_Stats;
    std::unique_ptr<IInputManager> m_Manager;
    std::vector<InputCallback> m_Callbacks;
    bool m_Initialized = false;
};

} // namespace Hydragon::Platform 