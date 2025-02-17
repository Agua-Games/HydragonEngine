/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include "imgui.h"
#include <string>

#include "ScriptEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowScriptEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    bool window_visible = ImGui::Begin("Script Editor", p_open, 
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);
    
    if (!window_visible)
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Script", "Ctrl+N")) {}
            if (ImGui::MenuItem("Import Script", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Recent Files"))
            {
                ImGui::MenuItem("player_controller.cpp");
                ImGui::MenuItem("ai_behavior.py");
                ImGui::MenuItem("game_rules.cpp");
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Send to External IDE", "Ctrl+E")) {}
            if (ImGui::MenuItem("Update Files from Ext. IDE", "Ctrl+R")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Copy from Node", "Ctrl+Shift+C")) {}
            if (ImGui::MenuItem("Paste to Node", "Ctrl+Shift+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Toggle Line Numbers", "Ctrl+L")) {}
            if (ImGui::MenuItem("Toggle Minimap", "Ctrl+M")) {}
            if (ImGui::MenuItem("Toggle Error List", "Ctrl+E")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Increase Font Size", "Ctrl++")) {}
            if (ImGui::MenuItem("Decrease Font Size", "Ctrl+-")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Script"))
        {
            if (ImGui::MenuItem("Compile", "F7")) {}
            if (ImGui::MenuItem("Run", "F5")) {}
            if (ImGui::MenuItem("Debug", "F9")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Record Macro")) {}
            if (ImGui::MenuItem("Play Macro")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Ask AI Assistant", "Ctrl+Space")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar
    ImGui::BeginChild("Toolbar", ImVec2(0, 30), true);
    {
        if (ImGui::Button("Compile")) {}
        ImGui::SameLine();
        if (ImGui::Button("Run")) {}
        ImGui::SameLine();
        if (ImGui::Button("Debug")) {}
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::Button("Find")) {}
        ImGui::SameLine();
        if (ImGui::Button("Replace")) {}
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();
        if (ImGui::Button("AI Assistant")) {}
    }
    ImGui::EndChild();

    // Main Content Area with Tabs
    static char editorBuffer[4096] = ""; // This should be replaced with a proper text editor implementation
    
    if (ImGui::BeginTabBar("ScriptTabs", ImGuiTabBarFlags_Reorderable))
    {
        // C++ Tab
        if (ImGui::BeginTabItem("player_controller.cpp"))
        {
            // Left Panel - Script Explorer
            ImGui::BeginChild("ScriptExplorer", ImVec2(200, 0), true);
            {
                ImGui::Text("Script Explorer");
                ImGui::Separator();
                
                if (ImGui::TreeNode("Source"))
                {
                    ImGui::Selectable("player_controller.cpp");
                    ImGui::Selectable("game_rules.cpp");
                    ImGui::TreePop();
                }
                
                if (ImGui::TreeNode("Headers"))
                {
                    ImGui::Selectable("player_controller.h");
                    ImGui::Selectable("game_rules.h");
                    ImGui::TreePop();
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Main Editor Area
            ImGui::BeginChild("EditorArea", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            {
                static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
                ImGui::InputTextMultiline("##source", editorBuffer, IM_ARRAYSIZE(editorBuffer), 
                    ImVec2(-1.0f, -1.0f), flags);
            }
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        // Python Tab
        if (ImGui::BeginTabItem("ai_behavior.py"))
        {
            ImGui::BeginChild("EditorArea");
            static char pythonBuffer[4096] = "";
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            ImGui::InputTextMultiline("##source", pythonBuffer, IM_ARRAYSIZE(pythonBuffer), 
                ImVec2(-1.0f, -1.0f), flags);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        // Add New Tab Button
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
        {
            ImGui::OpenPopup("NewScriptPopup");
        }

        if (ImGui::BeginPopup("NewScriptPopup"))
        {
            if (ImGui::MenuItem("New C++ Script")) {}
            if (ImGui::MenuItem("New Python Script")) {}
            ImGui::EndPopup();
        }

        ImGui::EndTabBar();
    }

    // Bottom Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, 20), true);
    {
        ImGui::Text("Line: 1 Col: 1");
        ImGui::SameLine(150);
        ImGui::Text("UTF-8");
        ImGui::SameLine(300);
        ImGui::Text("C++");
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Text("Ready");
    }
    ImGui::EndChild();

    ImGui::End();
}
} // namespace hdImgui
