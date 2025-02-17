/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "FileExplorer.h"
#include "hdImgui.h"
#include "fmt/core.h"
#include "fmt/format.h"

namespace hdImgui {
void ShowFileExplorer(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("File Explorer", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New File", "Ctrl+N")) {}
                if (ImGui::MenuItem("New Folder", "Ctrl+Shift+N")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::MenuItem("Open Recent"))
                {
                    // TODO: Show recent files popup
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Import", "Ctrl+I")) {}
                if (ImGui::MenuItem("Export", "Ctrl+E")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Select All", "Ctrl+A")) {}
                if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Icons View")) {}
                if (ImGui::MenuItem("List View")) {}
                if (ImGui::MenuItem("Details View")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Show Hidden Files")) {}
                if (ImGui::MenuItem("Show File Extensions")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Toolbar
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
        ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetWindowWidth(), 40), true);
        {
            if (ImGui::Button("Back")) {}
            ImGui::SameLine();
            if (ImGui::Button("Forward")) {}
            ImGui::SameLine();
            if (ImGui::Button("Up")) {}
            ImGui::SameLine();
            ImGui::Separator();
            ImGui::SameLine();

            // Path bar with breadcrumbs
            static char path[1024] = "D:/Projects/MyGame/Assets/";
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 100);
            ImGui::InputText("##Path", path, IM_ARRAYSIZE(path));
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {}
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();

        // Main content area
        ImGui::BeginChild("FileExplorerContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        {
            // Left panel containing roots, bookmarks and tree view
            static float leftPanelWidth = 250.0f;
            const float minLeftPanelWidth = 150.0f;
            const float maxLeftPanelWidth = 400.0f;

            ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), false);
            {
                // Root Locations Frame
                ImGui::BeginChild("RootLocations", ImVec2(0, 120), true);
                {
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "ROOT LOCATIONS");
                    ImGui::Separator();
                    if (ImGui::Selectable("C: System", false)) {}
                    if (ImGui::Selectable("D: Projects", true)) {}
                    if (ImGui::Selectable("Network", false)) {}
                    if (ImGui::Selectable("Cloud Storage", false)) {}
                }
                ImGui::EndChild();

                ImGui::Spacing();

                // Bookmarks Frame
                ImGui::BeginChild("Bookmarks", ImVec2(0, 150), true);
                {
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "BOOKMARKS");
                    ImGui::Separator();
                    if (ImGui::Selectable("Recent", false)) {}
                    if (ImGui::Selectable("Desktop", false)) {}
                    if (ImGui::Selectable("Documents", false)) {}
                    if (ImGui::Selectable("Downloads", false)) {}
                    if (ImGui::Selectable("Project Assets", true)) {}
                    if (ImGui::Button("Add Bookmark", ImVec2(-1, 0))) {}
                }
                ImGui::EndChild();

                ImGui::Spacing();

                // File Tree Frame
                ImGui::BeginChild("FileTree", ImVec2(0, 0), true);
                {
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "FILE TREE");
                    ImGui::Separator();
                    
                    // Example tree structure
                    if (ImGui::TreeNode("Assets"))
                    {
                        if (ImGui::TreeNode("Models"))
                        {
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Textures"))
                        {
                            ImGui::TreePop();
                        }
                        if (ImGui::TreeNode("Materials"))
                        {
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Left panel resizer
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
            ImGui::Button("##LeftSplitter", ImVec2(8.0f, -1));
            ImGui::PopStyleColor(3);

            if (ImGui::IsItemActive())
            {
                float deltaX = ImGui::GetIO().MouseDelta.x;
                leftPanelWidth += deltaX;
                if (leftPanelWidth < minLeftPanelWidth) leftPanelWidth = minLeftPanelWidth;
                if (leftPanelWidth > maxLeftPanelWidth) leftPanelWidth = maxLeftPanelWidth;
            }
            else if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::SameLine();

            // Main file view area
            static float sidebarWidth = 300.0f;
            const float minSidebarWidth = 200.0f;
            const float maxSidebarWidth = 500.0f;

            ImGui::BeginChild("MainFileView", ImVec2(ImGui::GetContentRegionAvail().x - sidebarWidth - 8.0f, 0), true);
            {
                // File view toolbar
                static int viewMode = 0; // 0: Icons, 1: List, 2: Details
                ImGui::RadioButton("Icons", &viewMode, 0); ImGui::SameLine();
                ImGui::RadioButton("List", &viewMode, 1); ImGui::SameLine();
                ImGui::RadioButton("Details", &viewMode, 2);
                
                ImGui::Separator();

                // Example files (in a grid for icon view)
                if (viewMode == 0) // Icons View
                {
                    float cellSize = 100.0f;
                    float panelWidth = ImGui::GetContentRegionAvail().x;
                    int columnCount = (int)(panelWidth / cellSize);
                    if (columnCount < 1) columnCount = 1;

                    if (ImGui::BeginTable("FilesTable", columnCount))
                    {
                        for (int i = 0; i < 20; i++)
                        {
                            ImGui::TableNextColumn();
                            if (ImGui::Selectable(fmt::format("File_{}.fbx", i).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(cellSize - 10, cellSize - 10)))
                            {
                                // Handle selection
                            }
                        }
                        ImGui::EndTable();
                    }
                }
                else if (viewMode == 1) // List View
                {
                    for (int i = 0; i < 20; i++)
                    {
                        if (ImGui::Selectable(fmt::format("File_{}.fbx", i).c_str()))
                        {
                            // Handle selection
                        }
                    }
                }
                else // Details View
                {
                    if (ImGui::BeginTable("FilesDetailsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
                    {
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableSetupColumn("Size");
                        ImGui::TableSetupColumn("Type");
                        ImGui::TableSetupColumn("Modified");
                        ImGui::TableHeadersRow();

                        for (int i = 0; i < 20; i++)
                        {
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            if (ImGui::Selectable(fmt::format("File_{}.fbx", i).c_str(), false, ImGuiSelectableFlags_SpanAllColumns))
                            {
                                // Handle selection
                            }
                            ImGui::TableNextColumn();
                            ImGui::Text("1.2 MB");
                            ImGui::TableNextColumn();
                            ImGui::Text("FBX File");
                            ImGui::TableNextColumn();
                            ImGui::Text("2024-02-20");
                        }
                        ImGui::EndTable();
                    }
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Right sidebar resizer
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
            ImGui::Button("##RightSplitter", ImVec2(8.0f, -1));
            ImGui::PopStyleColor(3);

            if (ImGui::IsItemActive())
            {
                float deltaX = ImGui::GetIO().MouseDelta.x;
                sidebarWidth -= deltaX;
                if (sidebarWidth < minSidebarWidth) sidebarWidth = minSidebarWidth;
                if (sidebarWidth > maxSidebarWidth) sidebarWidth = maxSidebarWidth;
            }
            else if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::SameLine();

            // Right sidebar with file properties and settings
            ImGui::BeginChild("RightSidebar", ImVec2(sidebarWidth, 0), true);
            {
                if (ImGui::BeginTabBar("SidebarTabs"))
                {
                    if (ImGui::BeginTabItem("Properties"))
                    {
                        ImGui::Text("File Properties");
                        ImGui::Separator();
                        ImGui::Text("Name: selected_file.fbx");
                        ImGui::Text("Size: 1.2 MB");
                        ImGui::Text("Created: 2024-02-20");
                        ImGui::Text("Modified: 2024-02-20");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Import Settings"))
                    {
                        ImGui::Text("Import Settings");
                        ImGui::Separator();
                        static bool generateLODs = true;
                        static bool importMaterials = true;
                        static float scale = 1.0f;
                        ImGui::Checkbox("Generate LODs", &generateLODs);
                        ImGui::Checkbox("Import Materials", &importMaterials);
                        ImGui::SliderFloat("Scale", &scale, 0.01f, 100.0f);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Export Settings"))
                    {
                        ImGui::Text("Export Settings");
                        ImGui::Separator();
                        static int format = 0;
                        static bool exportMaterials = true;
                        ImGui::Combo("Format", &format, "FBX\0OBJ\0USD\0GLTF\0\0");
                        ImGui::Checkbox("Export Materials", &exportMaterials);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Preview"))
                    {
                        ImGui::Text("File Preview");
                        ImGui::Separator();
                        // Add preview window here
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
} // namespace hdImgui
