/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {

// Forward declarations
struct HdEditorWindowData;

// Task types
enum class TaskType {
    HTN,    // Hierarchical Task Network
    GOAP,   // Goal Oriented Action Planning
    AGENT   // AI Agent Task
};

// Task status
enum class TaskStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    FAILED
};

// Basic task structure
struct Task {
    std::string name;
    std::string description;
    TaskType type;
    TaskStatus status;
    float progress;
    std::vector<std::string> prerequisites;
    std::vector<std::string> effects;
    bool isExpanded;
};

// Main editor function
void ShowAITaskEditor(bool* p_open, HdEditorWindowData* windowData);

} // namespace hdImgui
