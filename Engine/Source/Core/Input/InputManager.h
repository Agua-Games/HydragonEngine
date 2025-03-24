/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InputManager.h
 * @brief Header file for the InputManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - InputManager is a singleton class that manages the input handling.
 * - It provides methods to check if a key is pressed, released, etc.
 * - It also provides methods to get the mouse, and other input device data.
 * - It also provides methods to set the input mode, like raw mouse motion, etc.
 */
#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Node.h"

namespace hd {

struct inputActionMapping {
    std::string name;       // Name of the action
    std::string category;   // Category of the action
    int key;                // Key that triggers the action
    int mouseButton;        // Mouse button that triggers the action
    int gamepadButton;      // Gamepad button that triggers the action
};
    
struct InputManagerInfo : public NodeInfo {
    InputManagerInfo() {
        NodeType = "Input/InputManager";
        
        inputs = {
            "InputMode",     // Input mode, like raw mouse motion, etc.
            "CursorMode",    // Cursor mode, like normal, hidden, disabled, captured, etc.
            "StickyKeys",    // Sticky keys, which means that keys will not be released until they are explicitly released.
            "StickyMouseButtons" // Sticky mouse buttons, which means that mouse buttons will not be released until they are explicitly released.
            };
        
        outputs = {
            "KeyState",      // Key state, like pressed, released, etc.
            "MouseState",    // Mouse state, like pressed, released, etc.
            "MousePosition", // Mouse position
            "MouseDelta"     // Mouse delta
        };
    }
};


class InputManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    static InputManager* getInstance();
    void init();              // Initialize the InputManager. This is called once, when the engine is initialized.
    void load();              // Load the InputManager. This is called once, when the engine is loaded.

    // === Processing ===
    // Event handling
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool isMouseButtonPressed(int button);
    bool isMouseButtonReleased(int button);
    glm::vec2 getMousePosition();
    glm::vec2 getMouseDelta();
    void setCursorMode(int mode);                  // Set the cursor mode, like normal, hidden, disabled, captured, etc.
    void setRawMouseMotion(bool enabled);          // Enable or disable raw mouse motion. Raw mouse motion is useful for precise mouse control.
    void setStickyKeys(bool enabled);               // Set sticky keys, which means that keys will not be released until they are explicitly released.
    void setStickyMouseButtons(bool enabled);      // Set sticky mouse buttons, which means that mouse buttons will not be released until they are explicitly released.

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // === Execution ===
    void update();            // Update the InputManager. This is called once per frame, before the engine is updated.

    // === Cleanup ===
    void unload();            // Unload the InputManager. This is called once, when the engine is unloaded.
    void cleanup();           // Cleanup the InputManager. This is called once, when the engine is cleaned up.
};

} // namespace hd