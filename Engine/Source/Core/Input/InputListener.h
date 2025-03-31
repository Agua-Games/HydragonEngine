/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InputListener.h
 * @brief Header file for the InputListener class.
 * 
 * ARCHITECTURAL NOTES:
 * - InputListener is a node class that listens for input events.
 * - It provides methods to map input actions to keys, mouse buttons, etc.
 * - It also provides methods to trigger actions based on input events.
 * 
 * @todo Fix and organize the architecture of the input system, specially how the input context works - how it modifies which keys override other in a given context, etc.
 */
#pragma once
#include <vector>
#include <string>
#include "InputManager.h"
#include "Node.h"

namespace hd {

struct InputListenerInfo : public NodeInfo {
    InputListenerInfo() {
        NodeType = "Input/InputListener";
        
        inputs = {
            "InputAction",   // Input action to be mapped
            "Key",           // Key that triggers the action
            "MouseButton",   // Mouse button that triggers the action
            "GamepadButton"  // Gamepad button that triggers the action
        };
        
        outputs = {
            "ActionTriggered"   // Action triggered by input event
        };
    }
};

class InputListener : public Node {
public:
    // === Structure Definitions ===
    struct inputActionMapping {
        std::string name;       // Name of the action
        std::string category;   // Category of the action
        int key;                // Key that triggers the action
        int mouseButton;        // Mouse button that triggers the action
        int gamepadButton;      // Gamepad button that triggers the action
    };

    struct inputAction {
        std::string name;       // Name of the action
        std::string category;   // Category of the action
        bool triggered;         // Whether the action has been triggered
    };

    enum class InputType {
        Key,
        MouseButton,
        GamepadButton
    };

    enum InputContextType {
        Menu,
        Gameplay,
        Editor
    };

    struct InputContext {
        std::string name;                                // Name of the context
        std::vector<inputActionMapping> mappings;        // Mappings for the context
    };

    // === Allocation, Initialization, Loading ===
    explicit InputListener(const InputListenerInfo& info = InputListenerInfo())
        : Node(info), InputListenerInfo(info) {}
    void initialize() override {}
    void load() override {}

    // Set default values
    std::vector<inputActionMapping> mappings;
    std::vector<inputAction> actions;             // Actions that have been triggered by input events. This is used to track the state of actions.
    std::vector<InputContext> contexts;           // Contexts that the input listener is listening to.

    InputListener& mapAction(const std::string& action, int key) {
        m_mappings.push_back({action, "default", key, -1, -1});
        return *this;
    }

    // === Processing ===
    void () override {
        auto* input = InputManager::getInstance();
        
        for (const auto& mapping : m_mappings) {
            if (input->isKeyPressed(mapping.key)) {
                triggerAction(mapping.name);
            }
        }
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InputListener() = default;     // Default destructor
    
private:
    // === Private Members ===
    std::vector<inputActionMapping> m_mappings;
};

} // namespace hd