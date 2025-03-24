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
        
        Inputs = {
            "InputAction",   // Input action to be mapped
            "Key",           // Key that triggers the action
            "MouseButton",   // Mouse button that triggers the action
            "GamepadButton"  // Gamepad button that triggers the action
        };
        
        Outputs = {
            "ActionTriggered"   // Action triggered by input event
        };
    }
};

class InputListener : public Node {
public:
    struct inputActionMapping {
        std::string name;       // Name of the action
        std::string category;   // Category of the action
        int key;                // Key that triggers the action
        int mouseButton;        // Mouse button that triggers the action
        int gamepadButton;      // Gamepad button that triggers the action
    };

    InputListener& mapAction(const std::string& action, int key) {
        m_mappings.push_back({action, "default", key, -1, -1});
        return *this;
    }

    void processNodeGraph() override {
        auto* input = InputManager::getInstance();
        
        for (const auto& mapping : m_mappings) {
            if (input->isKeyPressed(mapping.key)) {
                triggerAction(mapping.name);
            }
        }
    }

private:
    std::vector<inputActionMapping> m_mappings;
};

} // namespace hd