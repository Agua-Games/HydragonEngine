/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include <string>

#include "Viewport2D.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowViewport2D(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Viewport 2D", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Import Image", "Ctrl+O")) {}
                if (ImGui::MenuItem("Export Image", "Ctrl+E")) {}
                if (ImGui::MenuItem("Save UV Layout", "Ctrl+S")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Recent Images"))
                {
                    ImGui::MenuItem("texture_01.png");
                    ImGui::MenuItem("normal_map.png");
                    ImGui::MenuItem("uv_layout.png");
                    ImGui::EndMenu();
                }
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
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Reset View", "F")) {}
                if (ImGui::MenuItem("Fit to Window", "A")) {}
                ImGui::Separator();
                ImGui::MenuItem("Show Grid", "G", &windowData->showViewport2DGrid);
                ImGui::MenuItem("Show UVs", "U", &windowData->showViewport2DUVs);
                ImGui::MenuItem("Show Axes", nullptr, &windowData->showViewport2DAxes);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Load Brush")) {}
                if (ImGui::MenuItem("Save Brush")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Load Stencil")) {}
                if (ImGui::MenuItem("Save Stencil")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left Toolbar (Vertical)
        ImGui::BeginChild("LeftToolbar", ImVec2(30, -ImGui::GetFrameHeightWithSpacing()), true);
        {
            // Tool buttons will go here (just placeholders for now)
            if (ImGui::Button("B", ImVec2(20, 20))) {} // Brush
            if (ImGui::Button("E", ImVec2(20, 20))) {} // Eraser
            if (ImGui::Button("S", ImVec2(20, 20))) {} // Select
            if (ImGui::Button("M", ImVec2(20, 20))) {} // Move
            if (ImGui::Button("R", ImVec2(20, 20))) {} // Rotate
            if (ImGui::Button("Z", ImVec2(20, 20))) {} // Zoom
            if (ImGui::Button("P", ImVec2(20, 20))) {} // Pan
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Main Viewport Area with Right Sidebar
        ImGui::BeginChild("ViewportWithSidebar", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false);
        {
            // Store the current sidebar width in a static variable
            static float sidebarWidth = 300.0f;
            const float minSidebarWidth = 200.0f;  // Minimum width
            const float maxSidebarWidth = 500.0f;  // Maximum width

            // Main Viewport
            ImGui::BeginChild("ViewportArea", ImVec2(ImGui::GetContentRegionAvail().x - sidebarWidth - 8.0f, 0), true);
            {
                // Top Controls Overlay
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.7f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));

                // Create a child window for the controls to ensure proper background
                ImGui::BeginChild("TopControls", ImVec2(ImGui::GetContentRegionAvail().x, 30), false, 
                                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                
                float startX = ImGui::GetCursorPosX() + 5;  // Add some padding
                float controlsY = ImGui::GetCursorPosY() + 5;
                float comboWidth = 100.0f;
                float comboOverlayCategorySpacing = 20.0f;

                // Brush Settings
                ImGui::SetCursorPos(ImVec2(startX, controlsY));
                ImGui::TextUnformatted("Brush"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(comboWidth);
                static int brushMode = 0;
                ImGui::Combo("##BrushMode", &brushMode, "Paint\0Erase\0Blur\0Smudge\0");

                // Brush Size
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                ImGui::TextUnformatted("Size"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(60);
                static float brushSize = 10.0f;
                ImGui::SliderFloat("##BrushSize", &brushSize, 1.0f, 100.0f, "%.0f");

                // Snap Settings
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                ImGui::TextUnformatted("Snap"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(comboWidth);
                static int snapMode = 0;
                ImGui::Combo("##SnapMode", &snapMode, "Grid\0Vertex\0Edge\0None\0");

                // Translate Increment
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                ImGui::TextUnformatted("Step"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(60);
                static float translateStep = 1.0f;
                ImGui::InputFloat("##TranslateStep", &translateStep, 0.1f, 1.0f, "%.1f");

                // Stencil Settings
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                ImGui::TextUnformatted("Stencil"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(comboWidth);
                static int stencilMode = 0;
                ImGui::Combo("##StencilMode", &stencilMode, "None\0Alpha\0Luma\0");

                // UV Settings
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                ImGui::TextUnformatted("UVs"); ImGui::SameLine(0, 4);
                ImGui::SetNextItemWidth(comboWidth);
                static int uvMode = 0;
                ImGui::Combo("##UVMode", &uvMode, "Show All\0Selected\0None\0");

                // Add Procedural Nodes Button in Top Controls
                ImGui::SameLine(0, comboOverlayCategorySpacing);
                if (ImGui::Button("+ Proc")) { /* Add procedural node */ }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Add Procedural Node");

                ImGui::EndChild();

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(3);

                // Actual viewport content would go here
                // For now, just a placeholder colored rect
                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImGui::GetCursorScreenPos(),
                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                           ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y),
                    IM_COL32(50, 50, 50, 255)
                );

                // Draw 2D axes if enabled
                if (windowData->showViewport2DAxes)
                {
                    ImVec2 origin = ImVec2(ImGui::GetCursorScreenPos().x + 50,
                                         ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y - 50);
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    
                    // X axis (red)
                    draw_list->AddLine(origin, 
                        ImVec2(origin.x + 50, origin.y), 
                        IM_COL32(255, 0, 0, 255), 2.0f);
                    draw_list->AddText(ImVec2(origin.x + 55, origin.y - 10), 
                        IM_COL32(255, 0, 0, 255), "X");

                    // Y axis (green)
                    draw_list->AddLine(origin, 
                        ImVec2(origin.x, origin.y - 50), 
                        IM_COL32(0, 255, 0, 255), 2.0f);
                    draw_list->AddText(ImVec2(origin.x - 10, origin.y - 55), 
                        IM_COL32(0, 255, 0, 255), "Y");
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Splitter
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
            ImGui::Button("##Splitter", ImVec2(8.0f, -1));
            ImGui::PopStyleColor(3);

            if (ImGui::IsItemActive())
            {
                float deltaX = ImGui::GetIO().MouseDelta.x;
                sidebarWidth = ImClamp<float>(sidebarWidth - deltaX, minSidebarWidth, maxSidebarWidth);
            }
            else if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::SameLine();

            // Right Sidebar
            ImGui::BeginChild("RightSidebar", ImVec2(sidebarWidth, 0), true, ImGuiWindowFlags_MenuBar);
            {
                // Right Sidebar Menu Bar
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("Layer"))
                    {
                        if (ImGui::MenuItem("New Layer")) {}
                        if (ImGui::MenuItem("New Layer From Selection")) {}
                        if (ImGui::MenuItem("Duplicate Layer")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Merge Down")) {}
                        if (ImGui::MenuItem("Merge Visible")) {}
                        if (ImGui::MenuItem("Flatten Image")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Adjust"))
                    {
                        if (ImGui::MenuItem("Levels")) {}
                        if (ImGui::MenuItem("Curves")) {}
                        if (ImGui::MenuItem("Brightness/Contrast")) {}
                        if (ImGui::MenuItem("Hue/Saturation")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Add Adjustment Layer")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Filter"))
                    {
                        if (ImGui::MenuItem("Add Procedural Filter")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Blur")) {}
                        if (ImGui::MenuItem("Sharpen")) {}
                        if (ImGui::MenuItem("Noise")) {}
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }

                // Tabs
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5));
                if (ImGui::BeginTabBar("SidebarTabs"))
                {
                    if (ImGui::BeginTabItem("Layers"))
                    {
                        // Layer controls
                        if (ImGui::Button("+")) {} ImGui::SameLine();
                        if (ImGui::Button("-")) {} ImGui::SameLine();
                        if (ImGui::Button("Proc")) {} // Add procedural layer
                        
                        // Layer list (example layers)
                        static bool layerVisible = true;
                        static float layerOpacity = 1.0f;
                        ImGui::Separator();
                        ImGui::Checkbox("##v1", &layerVisible); ImGui::SameLine();
                        ImGui::SetNextItemWidth(100);
                        ImGui::SliderFloat("##op1", &layerOpacity, 0.0f, 1.0f, "%.2f");
                        ImGui::SameLine();
                        ImGui::Text("Layer 1");
                        
                        // Layer blend modes
                        static int blendMode = 0;
                        ImGui::SetNextItemWidth(120);
                        ImGui::Combo("##blend", &blendMode, "Normal\0Multiply\0Screen\0Overlay\0");
                        ImGui::EndTabItem();
                    }
                    
                    if (ImGui::BeginTabItem("Channels"))
                    {
                        static bool rgbaChannels[4] = {true, true, true, true};
                        ImGui::Checkbox("R", &rgbaChannels[0]);
                        ImGui::Checkbox("G", &rgbaChannels[1]);
                        ImGui::Checkbox("B", &rgbaChannels[2]);
                        ImGui::Checkbox("A", &rgbaChannels[3]);
                        ImGui::Separator();
                        if (ImGui::Button("Save Channel As...")) {}
                        if (ImGui::Button("Load Into Channel...")) {}
                        ImGui::EndTabItem();
                    }
                    
                    if (ImGui::BeginTabItem("Texture Sets"))
                    {
                        // Texture set controls
                        if (ImGui::Button("New Set")) {}
                        if (ImGui::Button("Import Set")) {}
                        
                        // Example texture set list
                        static int currentSet = 0;
                        ImGui::Separator();
                        ImGui::RadioButton("Base Color", &currentSet, 0);
                        ImGui::RadioButton("Normal Map", &currentSet, 1);
                        ImGui::RadioButton("Roughness", &currentSet, 2);
                        ImGui::RadioButton("Metallic", &currentSet, 3);
                        ImGui::RadioButton("Ambient Occlusion", &currentSet, 4);
                        ImGui::RadioButton("Height", &currentSet, 5);
                        ImGui::RadioButton("Emission", &currentSet, 6);
                        
                        ImGui::Separator();
                        if (ImGui::Button("Generate Missing Maps")) {}
                        if (ImGui::Button("Bake Selected")) {}
                        ImGui::EndTabItem();
                    }
                    
                    if (ImGui::BeginTabItem("Settings"))
                    {
                        // Image settings
                        ImGui::Text("Image Properties");
                        static int width = 1024, height = 1024;
                        static int bits = 8;
                        static int colorSpace = 0;
                        
                        ImGui::InputInt("Width", &width);
                        ImGui::InputInt("Height", &height);
                        ImGui::Combo("Bit Depth", &bits, "8-bit\016-bit\032-bit\0");
                        ImGui::Combo("Color Space", &colorSpace, "sRGB\0Linear\0");
                        
                        ImGui::Separator();
                        ImGui::Text("Canvas Settings");
                        static float zoom = 100.0f;
                        static int rotation = 0;
                        ImGui::SliderFloat("Zoom", &zoom, 1.0f, 1000.0f, "%.1f%%");
                        ImGui::SliderInt("Rotation", &rotation, 0, 360, "%d°");
                        
                        ImGui::Separator();
                        if (ImGui::Button("Reset View")) {}
                        if (ImGui::Button("Fit to Window")) {}
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Procedural"))
                    {
                        if (ImGui::Button("Add Noise Generator")) {}
                        if (ImGui::Button("Add Pattern Generator")) {}
                        if (ImGui::Button("Add Fractal Generator")) {}
                        ImGui::Separator();
                        if (ImGui::Button("Add Color Adjustment")) {}
                        if (ImGui::Button("Add Filter Node")) {}
                        if (ImGui::Button("Add Blend Node")) {}
                        ImGui::Separator();
                        if (ImGui::Button("Show Node Graph")) {}
                        ImGui::EndTabItem();
                    }
                    
                    ImGui::EndTabBar();
                }
                ImGui::PopStyleVar();
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

        // Bottom Status Bar
        ImGui::BeginChild("StatusBar", ImVec2(0, ImGui::GetFrameHeightWithSpacing()), true);
        {
            ImGui::Text("Zoom: 100%%");
            ImGui::SameLine(150);
            ImGui::Text("Pos: 0, 0");
            ImGui::SameLine(300);
            ImGui::Text("Size: 1024x1024");
            ImGui::SameLine(ImGui::GetWindowWidth() - 100);
            ImGui::Text("Ready");
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
} // namespace hdImgui
