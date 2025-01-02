/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Input system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Input/InputSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct InputBindingsConfig {
    bool enableRawInput = true;
    bool enableHapticFeedback = true;
    bool enableGestureRecognition = true;
    bool enableInputRecording = true;
    uint32_t maxInputCallbacks = 100;
    uint32_t maxGesturePatterns = 50;
    uint32_t recordingBufferSize = 1000;
    std::string inputMappingPath = "Config/Input";
};

class InputBindings {
public:
    static InputBindings& Get();
    
    void Initialize(const InputBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Action management
    ActionHandle CreateAction(const std::string& name, const ActionDesc& desc);
    void DestroyAction(ActionHandle handle);
    void BindAction(ActionHandle handle, const ActionBinding& binding);
    void UnbindAction(ActionHandle handle);
    
    // Input state
    bool IsActionTriggered(const std::string& name) const;
    float GetActionValue(const std::string& name) const;
    Vector2 GetPointerPosition() const;
    
    // Input callbacks
    void RegisterActionCallback(const std::string& name, const ActionCallback& callback);
    void UnregisterActionCallback(const std::string& name);
    
    // Gesture recognition
    void RegisterGesturePattern(const std::string& name, const GesturePattern& pattern);
    void UnregisterGesturePattern(const std::string& name);
    
    // Input recording
    void StartInputRecording();
    void StopInputRecording();
    void PlaybackRecording(const std::string& path);
    void SaveRecording(const std::string& path);
    
    // Haptic feedback
    void TriggerHapticFeedback(const std::string& device, const HapticEffect& effect);
    
    const InputBindingsStats& GetStats() const { return m_Stats; }

private:
    InputBindings() = default;
    
    InputBindingsConfig m_Config;
    InputBindingsStats m_Stats;
    std::unordered_map<std::string, ActionHandle> m_ActionMap;
    std::unordered_map<std::string, ActionCallback> m_Callbacks;
    std::unordered_map<std::string, GesturePattern> m_GesturePatterns;
    std::vector<InputEvent> m_RecordingBuffer;
    std::unique_ptr<Input::IInputProcessor> m_Processor;
    std::unique_ptr<Input::IGestureRecognizer> m_GestureRecognizer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 