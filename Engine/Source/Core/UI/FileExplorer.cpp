/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

#include "fmt/core.h"
#include "fmt/format.h"
#include "IconsFontAwesome6.h"
#include "IconsMaterialSymbols.h"

#include "FileExplorer.h"
#include "hdImgui.h"
#include "ResourceManager.h"

namespace fs = std::filesystem;

namespace hdImgui {

namespace {
    static char searchBuffer[256] = "";
    static char currentPath[1024] = "C:/";
    static std::string selectedFile;
    static std::string draggedFilePath;
    static float previewPanelWidth = 300.0f;
    static float placesWidth = 150.0f;
    static float settingsPanelHeight = 200.0f;
    
    // Quick access locations
    struct QuickAccessEntry {
        const char* name;
        const char* path;
        const char* icon;
    };

    static QuickAccessEntry quickAccessLocations[] = {
        {"Desktop", "C:/Users/Username/Desktop", ICON_FA_DESKTOP},
        {"Documents", "C:/Users/Username/Documents", ICON_FA_FOLDER},
        {"Downloads", "C:/Users/Username/Downloads", ICON_FA_DOWNLOAD},
        {"Pictures", "C:/Users/Username/Pictures", ICON_FA_IMAGE},
        {"Music", "C:/Users/Username/Music", ICON_FA_MUSIC},
        {"Videos", "C:/Users/Username/Videos", ICON_FA_VIDEO}
    };

    static const char* drives[] = {
        "C:", "D:", "E:", "F:"
    };
}

// Forward declarations
static void RenderPlacesPanel();
static void RenderFileListAndPreview();
static void RenderFileList();
static void RenderPreviewPanel();
static void RenderSettingsPanel();

// Implementation of RenderPlacesPanel
static void RenderPlacesPanel()
{
    // Drives section
    if (ImGui::CollapsingHeader("Drives", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (const char* drive : drives)
        {
            ImGui::PushID(drive);
            if (ImGui::Selectable(fmt::format("{} {}", ICON_FA_HARD_DRIVE, drive).c_str()))
            {
                strcpy_s(currentPath, sizeof(currentPath), drive);
                strcat_s(currentPath, sizeof(currentPath), "/");
            }
            ImGui::PopID();
        }
    }

    // Quick access section
    if (ImGui::CollapsingHeader("Quick Access", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (const auto& location : quickAccessLocations)
        {
            ImGui::PushID(location.path);
            if (ImGui::Selectable(fmt::format("{} {}", location.icon, location.name).c_str()))
            {
                strcpy_s(currentPath, sizeof(currentPath), location.path);
            }
            ImGui::PopID();
        }
    }
}

// Implementation of RenderFileList
static void RenderFileList()
{
    // Column headers
    ImGui::Columns(4, "FileListColumns");
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Size"); ImGui::NextColumn();
    ImGui::Text("Type"); ImGui::NextColumn();
    ImGui::Text("Modified"); ImGui::NextColumn();
    ImGui::Separator();

    // TODO: Implement actual file listing
    // Placeholder items
    for (int i = 0; i < 10; i++)
    {
        ImGui::Text("File %d", i); ImGui::NextColumn();
        ImGui::Text("1 KB"); ImGui::NextColumn();
        ImGui::Text("File"); ImGui::NextColumn();
        ImGui::Text("2023-01-01"); ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

// Implementation of RenderPreviewPanel
static void RenderPreviewPanel()
{
    ImGui::Text("Preview");
    ImGui::Separator();
    
    // Center the "No Preview Available" text
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImVec2 textSize = ImGui::CalcTextSize("No Preview Available");
    ImGui::SetCursorPos(ImVec2(
        (windowSize.x - textSize.x) * 0.5f,
        (windowSize.y - textSize.y) * 0.5f
    ));
    ImGui::TextDisabled("No Preview Available");
}

// Implementation of RenderFileListAndPreview
static void RenderFileListAndPreview()
{
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float fileListWidth = contentSize.x - previewPanelWidth - ImGui::GetStyle().ItemSpacing.x;

    // File list
    if (fileListWidth > 0)
    {
        ImGui::BeginChild("FileList", ImVec2(fileListWidth, 0), true);
        RenderFileList();
        ImGui::EndChild();

        ImGui::SameLine();

        // Preview panel
        ImGui::BeginChild("PreviewPanel", ImVec2(previewPanelWidth, 0), true);
        RenderPreviewPanel();
        ImGui::EndChild();
    }
}

// Implementation of RenderSettingsPanel
static void RenderSettingsPanel()
{
    if (ImGui::BeginTabBar("SettingsTabs"))
    {
        if (ImGui::BeginTabItem("General"))
        {
            ImGui::Checkbox("Show Hidden Files", nullptr);
            ImGui::Checkbox("Show File Extensions", nullptr);
            ImGui::Checkbox("Show Preview Panel", nullptr);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("View"))
        {
            ImGui::Text("View Mode:");
            ImGui::RadioButton("List", nullptr, true);
            ImGui::RadioButton("Icons", nullptr, false);
            ImGui::RadioButton("Details", nullptr, false);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Filters"))
        {
            ImGui::Text("File Filters:");
            ImGui::InputText("Extension Filter", nullptr, 0);
            ImGui::InputText("Name Filter", nullptr, 0);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

// Main function implementation
void ShowFileExplorer(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !windowData) return;

    ImGui::SetNextWindowSize(ImVec2(1000, 600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);

    if (!ImGui::Begin("File Explorer", p_open))
    {
        ImGui::End();
        return;
    }

    // Main layout
    ImGui::BeginChild("MainLayout", ImVec2(0, -settingsPanelHeight));
    {
        // Left panel (Places)
        ImGui::BeginChild("Places", ImVec2(placesWidth, 0), true);
        ImGui::Text("Places Panel");
        ImGui::EndChild();

        ImGui::SameLine();

        // Right side (containing file list and preview)
        ImGui::BeginChild("RightSide");
        {
            // Toolbar
            ImGui::Text("Current Path: %s", currentPath);
            ImGui::Text("Search: %s", searchBuffer);

            // File list and preview area
            float contentWidth = ImGui::GetContentRegionAvail().x;
            float fileListWidth = contentWidth - previewPanelWidth - ImGui::GetStyle().ItemSpacing.x;

            ImGui::BeginChild("FileList", ImVec2(fileListWidth, 0), true);
            ImGui::Text("File List Here");
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("Preview", ImVec2(0, 0), true);
            ImGui::Text("Preview Panel");
            ImGui::EndChild();
        }
        ImGui::EndChild(); // RightSide
    }
    ImGui::EndChild(); // MainLayout

    // Settings panel at bottom
    ImGui::BeginChild("Settings", ImVec2(0, 0), true);
    ImGui::Text("Settings Panel");
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui


