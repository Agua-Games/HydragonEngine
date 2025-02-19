/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>
#include <vector>
#include <algorithm>  // for std::clamp

#include <imgui.h>
#include <imgui_internal.h>
#include "IconsMaterialSymbols.h"

#include "PluginEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct PluginSettings {
    // Plugin info
    std::string selectedPlugin;
    bool autoLoad = true;
    bool enableHotReload = true;
    bool showCompatibilityWarnings = true;

    // Categories
    bool showInstalled = true;
    bool showAvailable = true;
    bool showUpdates = false;
    bool showDependencies = true;
};

static void ShowPluginToolbar(PluginSettings& settings)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

    // Plugin Management
    if (ImGui::Button(ICON_MS_ADD_CIRCLE "##Add", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Plugin");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_DELETE "##Remove", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Remove Plugin");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

    // Plugin Operations
    if (ImGui::Button(ICON_MS_REFRESH "##Reload", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reload Plugin");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_SETTINGS "##Settings", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Plugin Settings");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

    // Import/Export
    if (ImGui::Button(ICON_MS_UPLOAD "##Export", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Export Plugin");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_DOWNLOAD "##Import", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Import Plugin");

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
}

void ShowPluginEditor(bool* p_open, HdEditorWindowData* windowData)
{
    static PluginSettings settings;
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 150.0f;
    const float maxLeftPanelWidth = 400.0f;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Plugin Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import Plugin...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Export Plugin...", "Ctrl+E")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Refresh All", "F5")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Show Installed", nullptr, &settings.showInstalled);
            ImGui::MenuItem("Show Available", nullptr, &settings.showAvailable);
            ImGui::MenuItem("Show Updates", nullptr, &settings.showUpdates);
            ImGui::MenuItem("Show Dependencies", nullptr, &settings.showDependencies);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Auto Load", nullptr, &settings.autoLoad);
            ImGui::MenuItem("Enable Hot Reload", nullptr, &settings.enableHotReload);
            ImGui::MenuItem("Show Compatibility Warnings", nullptr, &settings.showCompatibilityWarnings);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ShowPluginToolbar(settings);
    ImGui::Separator();

    // Left Panel - Plugin List and Categories
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    {
        // Search bar
        static char searchBuffer[128] = "";
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search Plugins...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

        ImGui::Separator();

        // Categories
        if (ImGui::CollapsingHeader("Installed"))
        {
            ImGui::Indent();
            ImGui::Text("Plugin 1");
            ImGui::Text("Plugin 2");
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Available"))
        {
            ImGui::Indent();
            ImGui::Text("Plugin 3");
            ImGui::Text("Plugin 4");
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Updates"))
        {
            ImGui::Indent();
            ImGui::Text("Plugin 1 (Update available)");
            ImGui::Unindent();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Resizer
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::Button("##Resizer", ImVec2(4.0f, -1));
    if (ImGui::IsItemActive())
    {
        leftPanelWidth += ImGui::GetIO().MouseDelta.x;
        leftPanelWidth = std::clamp(leftPanelWidth, minLeftPanelWidth, maxLeftPanelWidth);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    ImGui::PopStyleColor();

    ImGui::SameLine();

    // Right Panel - Details and Settings
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    {
        if (ImGui::BeginTabBar("PluginTabs"))
        {
            if (ImGui::BeginTabItem("Details"))
            {
                ImGui::Text("Plugin Details");
                ImGui::Separator();
                
                ImGui::Text("Name: Sample Plugin");
                ImGui::Text("Version: 1.0.0");
                ImGui::Text("Author: John Doe");
                ImGui::Text("Description: A sample plugin description");
                
                ImGui::Separator();
                ImGui::Text("Dependencies:");
                ImGui::BulletText("Core SDK >= 1.0.0");
                ImGui::BulletText("Graphics Module >= 2.1.0");

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                ImGui::Text("Plugin Settings");
                ImGui::Separator();
                
                ImGui::Checkbox("Auto Load", &settings.autoLoad);
                ImGui::Checkbox("Enable Hot Reload", &settings.enableHotReload);
                ImGui::Checkbox("Show Compatibility Warnings", &settings.showCompatibilityWarnings);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Node Graph"))
            {
                ImGui::Text("Plugin Node Graph will be integrated here");
                ImGui::Text("(Using existing NodeGraphEditor)");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
