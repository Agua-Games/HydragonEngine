/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIManager.h
 * @brief Header file for the UIManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - UIManager is a singleton class that manages the user interface in the game.
 * - It is responsible for handling user input, rendering UI elements, and managing UI state.
 * - It uses the ImGui library for UI rendering and interaction.
 * 
 * @todo Study if it's the case of wrapping Imgui's main functions into methods to avoid the lower capital case mess imgui uses for functions. Because if makes the codebase
 * much cleaner and consistent, but then we'll have to wrap all the many imgui functions in our own methods. Anyway, it makes sense for future flexibility - changing to
 * Qt or other UI libraries.
 * @todo Create .cpp file and move the implementation there.
 * @todo Organize the existing code into logical sections and functions.
 * @todo Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * @todo Flesh out the class and its methods, structs, enums, etc.
 */
#pragma once
#include <imgui.h>
#include "Node.h"

namespace hd {

struct UIManagerInfo : public NodeInfo {
    UIManagerInfo() {
        NodeType = "UI/UIManager";
        inputs = {
            "UIElements" // Array of UI elements
        };
        outputs = {
            "UIState" // UI state
        };
    }
};

class UIManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit UIManager(const UIManagerInfo& info = UIManagerInfo())
        : Node(info), UIManagerInfo(info) {}   
    void initialize() override {}
    void load() override {}

    // === Processing ===
    void processNode() override {}    // Process UI elements and update state
    void update() override {}     // Update UI state
    void render() override {}     // Render UI elements

    // === Cleanup ===
    void unload() override {}    // Unload UI elements and cleanup state
    void cleanup() override {}   // Cleanup UI state and resources
    ~UIManager() = default;     // Default destructor
};

} // namespace hd