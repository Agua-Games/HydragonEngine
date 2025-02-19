/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>
#include <vector>
#include <algorithm>  // for std::clamp

#include <imgui.h>
#include "IconsMaterialSymbols.h"

#include "ExtensionsEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct ExtensionSettings {
    // Extension filters
    bool showEnabled = true;
    bool showDisabled = true;
    bool showUpdates = true;
    bool showPopular = false;
    bool showRecommended = true;

    // View options
    bool compactView = false;
    bool showDescriptions = true;
    bool groupByCategory = true;

    // Extension info
    std::string selectedExtension;
    std::string searchQuery;
};

static void ShowExtensionToolbar(ExtensionSettings& settings)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

    // Extension Management
    if (ImGui::Button(ICON_MS_EXTENSION "##Install", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Install Extension");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_DELETE "##Remove", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Remove Extension");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

    // Extension Operations
    if (ImGui::Button(ICON_MS_REFRESH "##Reload", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reload Extensions");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_UPDATE "##Update", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Check for Updates");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

    // View Options
    if (ImGui::Button(ICON_MS_VIEW_LIST "##ViewMode", ImVec2(30, 30))) 
        settings.compactView = !settings.compactView;
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(settings.compactView ? "Switch to Detailed View" : "Switch to Compact View");

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
}

static void ShowExtensionCard(const char* name, const char* description, const char* version, bool enabled)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
    ImGui::BeginChild(name, ImVec2(-1, 80), true);
    
    // Extension name and version
    ImGui::Text("%s", name);
    ImGui::SameLine();
    ImGui::TextDisabled("v%s", version);
    ImGui::SameLine(ImGui::GetWindowWidth() - 60);
    
    // Enable/Disable toggle
    bool isEnabled = enabled;
    if (ImGui::Checkbox("##enabled", &isEnabled))
    {
        // Toggle logic will go here
    }

    // Description
    ImGui::TextWrapped("%s", description);
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void ShowExtensionsEditor(bool* p_open, HdEditorWindowData* windowData)
{
    static ExtensionSettings settings;
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 150.0f;
    const float maxLeftPanelWidth = 400.0f;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Extensions", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Show Enabled", nullptr, &settings.showEnabled);
            ImGui::MenuItem("Show Disabled", nullptr, &settings.showDisabled);
            ImGui::MenuItem("Show Updates", nullptr, &settings.showUpdates);
            ImGui::MenuItem("Show Popular", nullptr, &settings.showPopular);
            ImGui::MenuItem("Show Recommended", nullptr, &settings.showRecommended);
            ImGui::Separator();
            ImGui::MenuItem("Compact View", nullptr, &settings.compactView);
            ImGui::MenuItem("Show Descriptions", nullptr, &settings.showDescriptions);
            ImGui::MenuItem("Group by Category", nullptr, &settings.groupByCategory);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ShowExtensionToolbar(settings);
    ImGui::Separator();

    // Left Panel - Categories and Filters
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    {
        // Search bar
        static char searchBuffer[128] = "";
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search Extensions...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

        ImGui::Separator();

        // Categories
        if (ImGui::TreeNode("Featured"))
        {
            ImGui::Selectable("Popular", false);
            ImGui::Selectable("Recommended", false);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Categories"))
        {
            ImGui::Selectable("Modeling", false);
            ImGui::Selectable("Animation", false);
            ImGui::Selectable("Rendering", false);
            ImGui::Selectable("UI/UX", false);
            ImGui::Selectable("Productivity", false);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Status"))
        {
            ImGui::Selectable("Enabled", false);
            ImGui::Selectable("Disabled", false);
            ImGui::Selectable("Updates Available", false);
            ImGui::TreePop();
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

    // Right Panel - Extension List and Details
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    {
        if (ImGui::BeginTabBar("ExtensionTabs"))
        {
            if (ImGui::BeginTabItem("Installed"))
            {
                ShowExtensionCard("Modeling Tools Pro", "Advanced modeling tools for professionals", "1.2.3", true);
                ShowExtensionCard("Animation Suite", "Comprehensive animation toolkit", "2.0.1", true);
                ShowExtensionCard("Scene Optimizer", "Optimize scene performance", "0.9.5", false);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Browse"))
            {
                ShowExtensionCard("Material Library", "Extensive material collection", "1.0.0", false);
                ShowExtensionCard("Terrain Generator", "Procedural terrain creation", "2.1.0", false);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Updates"))
            {
                ShowExtensionCard("Modeling Tools Pro", "Update available: v1.2.4", "1.2.3", true);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
