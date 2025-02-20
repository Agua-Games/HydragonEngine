/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>

#include <imgui.h>
#include "IconsMaterialSymbols.h"

#include "AITaskEditor.h"
#include "hdImgui.h"

namespace hdImgui {

namespace {  // Anonymous namespace for internal state
    struct AITaskEditorState {
        std::vector<Task> tasks;
        TaskType currentTaskType = TaskType::HTN;
        int selectedTaskIndex = -1;
        bool showTaskDetails = false;
        bool showMetrics = true;
        bool autoConnect = true;
    };

    static AITaskEditorState state;
}

static void ShowTaskTypeSelector() {
    ImGui::Text("Task Type:");
    ImGui::SameLine();
    
    const char* types[] = { "HTN", "GOAP", "Agent Task" };
    int currentType = static_cast<int>(state.currentTaskType);
    
    if (ImGui::Combo("##TaskType", &currentType, types, IM_ARRAYSIZE(types))) {
        state.currentTaskType = static_cast<TaskType>(currentType);
    }
}

static void ShowTaskList() {
    ImGui::BeginChild("TaskList", ImVec2(200, 0), true);
    ImGui::Text("Tasks");
    ImGui::Separator();

    for (size_t i = 0; i < state.tasks.size(); i++) {
        Task& task = state.tasks[i];
        if (ImGui::Selectable(task.name.c_str(), state.selectedTaskIndex == static_cast<int>(i))) {
            state.selectedTaskIndex = static_cast<int>(i);
            state.showTaskDetails = true;
        }
    }
    ImGui::EndChild();
}

static void ShowTaskDetails() {
    if (state.selectedTaskIndex >= 0 && state.selectedTaskIndex < state.tasks.size()) {
        Task& task = state.tasks[state.selectedTaskIndex];
        
        ImGui::BeginChild("TaskDetails", ImVec2(0, 0), true);
        
        // Task header
        ImGui::Text("Task Details");
        ImGui::Separator();

        // Basic properties
        static char nameBuffer[256];
        strcpy_s(nameBuffer, sizeof(nameBuffer), task.name.c_str());
        ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer));
        
        static char descBuffer[1024];
        strcpy_s(descBuffer, sizeof(descBuffer), task.description.c_str());
        ImGui::InputTextMultiline("Description", descBuffer, sizeof(descBuffer));

        // Status and progress
        const char* statusStrings[] = { "Pending", "In Progress", "Completed", "Failed" };
        int statusIndex = static_cast<int>(task.status);
        ImGui::Combo("Status", &statusIndex, statusStrings, IM_ARRAYSIZE(statusStrings));
        
        ImGui::SliderFloat("Progress", &task.progress, 0.0f, 1.0f, "%.2f");

        // Prerequisites and Effects
        if (ImGui::CollapsingHeader("Prerequisites", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto& prereq : task.prerequisites) {
                ImGui::BulletText("%s", prereq.c_str());
            }
            if (ImGui::Button(ICON_MS_ADD " Add Prerequisite")) {
                task.prerequisites.push_back("New Prerequisite");
            }
        }

        if (ImGui::CollapsingHeader("Effects", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto& effect : task.effects) {
                ImGui::BulletText("%s", effect.c_str());
            }
            if (ImGui::Button(ICON_MS_ADD " Add Effect")) {
                task.effects.push_back("New Effect");
            }
        }

        ImGui::EndChild();
    }
    // If condition is false, we never call EndChild()
    // Add an else clause to handle this case
    else {
        ImGui::BeginChild("TaskDetails", ImVec2(0, 0), true);
        ImGui::Text("No task selected");
        ImGui::EndChild();
    }
}

void ShowAITaskEditor(bool* p_open, HdEditorWindowData* windowData) {
    if (!p_open || !*p_open)
        return;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    bool window_visible = ImGui::Begin("AI Task", p_open, ImGuiWindowFlags_MenuBar);
    if (!window_visible)
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem(ICON_MS_ADD_BOX " New Task")) {
                Task newTask;
                newTask.name = "New Task";
                newTask.description = "Task description";
                newTask.type = state.currentTaskType;
                newTask.status = TaskStatus::PENDING;
                newTask.progress = 0.0f;
                state.tasks.push_back(newTask);
            }
            if (ImGui::MenuItem(ICON_MS_SAVE " Save Tasks")) {}
            if (ImGui::MenuItem(ICON_MS_FOLDER_OPEN " Load Tasks")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_EXIT_TO_APP " Exit")) {
                *p_open = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show Metrics", nullptr, &state.showMetrics);
            ImGui::MenuItem("Auto-Connect to NodeGraph", nullptr, &state.autoConnect);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImVec2 buttonSize(32, 32);
    
    if (ImGui::Button(ICON_MS_ADD "##New", buttonSize)) {
        Task newTask;
        newTask.name = "New Task";
        newTask.description = "Task description";
        newTask.type = state.currentTaskType;
        newTask.status = TaskStatus::PENDING;
        newTask.progress = 0.0f;
        state.tasks.push_back(newTask);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Task");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_DELETE "##Delete", buttonSize)) {
        if (state.selectedTaskIndex >= 0 && state.selectedTaskIndex < state.tasks.size()) {
            state.tasks.erase(state.tasks.begin() + state.selectedTaskIndex);
            state.selectedTaskIndex = -1;
            state.showTaskDetails = false;
        }
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Task");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_EDIT "##Edit", buttonSize)) {
        state.showTaskDetails = true;
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Edit Task");
    ImGui::SameLine();
    
    ImGui::Dummy(ImVec2(10, 0));
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SHARE "##NodeGraph", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open in NodeGraph");
    
    ImGui::PopStyleVar();
    ImGui::Separator();

    // Task Type Selector
    ShowTaskTypeSelector();
    
    // Main Content
    ImGui::BeginChild("MainContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
    // Left panel - Task List
    ShowTaskList();
    
    ImGui::SameLine();
    
    // Right panel - Task Details
    ShowTaskDetails();
    
    ImGui::EndChild();  // End MainContent

    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, ImGui::GetFrameHeightWithSpacing()), true);
    ImGui::Text("Tasks: %zu", state.tasks.size());
    ImGui::SameLine();
    ImGui::Text("Selected: %d", state.selectedTaskIndex);
    ImGui::EndChild();  // End StatusBar

    ImGui::End();
}

} // namespace hdImgui
