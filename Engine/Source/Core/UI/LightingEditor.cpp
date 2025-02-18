/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include "LightingEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

void ShowLightingEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open) return;
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Lighting Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Light Setup", "Ctrl+N")) {}
                if (ImGui::MenuItem("Import Light Setup...", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save Setup", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save Setup As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Recent Light Setups"))
                {
                    ImGui::MenuItem("indoor_lighting.hdlight");
                    ImGui::MenuItem("outdoor_day.hdlight");
                    ImGui::MenuItem("night_scene.hdlight");
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Export Light Data...")) {}
                if (ImGui::MenuItem("Import From HDR...")) {}
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
                if (ImGui::MenuItem("Select All Lights", "Ctrl+A")) {}
                if (ImGui::MenuItem("Select By Type")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Create"))
            {
                if (ImGui::MenuItem("Ambient Light")) {}
                if (ImGui::MenuItem("Directional Light")) {}
                if (ImGui::MenuItem("Point Light")) {}
                if (ImGui::MenuItem("Spot Light")) {}
                if (ImGui::MenuItem("Area Light")) {}
                if (ImGui::MenuItem("Volume Light")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Light Group")) {}
                if (ImGui::MenuItem("Light Probe")) {}
                if (ImGui::MenuItem("Reflection Probe")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Light Painter")) {}
                if (ImGui::MenuItem("Auto Light Placement")) {}
                if (ImGui::MenuItem("Light Optimizer")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Bake Lighting", "Ctrl+B")) {}
                if (ImGui::MenuItem("Bake Settings...")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Generate Light Probes")) {}
                if (ImGui::MenuItem("Update All Probes")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Show Light Gizmos", "Ctrl+G")) {}
                if (ImGui::MenuItem("Show Light Volumes")) {}
                if (ImGui::MenuItem("Show Shadow Cascades")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Lighting Only View")) {}
                if (ImGui::MenuItem("Shadow Only View")) {}
                if (ImGui::MenuItem("Light Complexity View")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Reset Layout")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Analysis"))
            {
                if (ImGui::MenuItem("Light Overlap Check")) {}
                if (ImGui::MenuItem("Shadow Quality Analysis")) {}
                if (ImGui::MenuItem("Performance Profiler")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Generate Light Report")) {}
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Top toolbar
        {
            const float toolbarHeight = 30.0f;

            // Style settings for toolbar
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            
            if (ImGui::BeginChild("LightingToolbar", ImVec2(-1, toolbarHeight), true, 
                ImGuiWindowFlags_NoScrollbar | 
                ImGuiWindowFlags_NoScrollWithMouse))
            {
                // Style for buttons
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

                // Ensure buttons start from the very top-left corner
                ImGui::SetCursorScreenPos(ImGui::GetWindowPos());

                // Light Creation Tools
                {
                    if (ImGui::Button(ICON_MS_ADD "##AddLight", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Light");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_CONTENT_COPY "##DuplicateLight", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Duplicate Light");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_DELETE "##DeleteLight", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Light");
                    ImGui::SameLine();

                    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
                }

                // File Operations
                {
                    if (ImGui::Button(ICON_MS_SAVE "##SaveLights", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save Light Setup");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_FOLDER_OPEN "##OpenLights", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open Light Setup");
                    ImGui::SameLine();

                    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
                }

                // Light Presets
                {
                    if (ImGui::Button(ICON_MS_LIGHTBULB "##LightPresets", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Light Presets");
                    if (ImGui::BeginPopupContextItem("LightPresets"))
                    {
                        if (ImGui::MenuItem("Indoor - Bright")) {}
                        if (ImGui::MenuItem("Indoor - Moody")) {}
                        if (ImGui::MenuItem("Outdoor - Sunny")) {}
                        if (ImGui::MenuItem("Outdoor - Overcast")) {}
                        if (ImGui::MenuItem("Night - Moonlit")) {}
                        if (ImGui::MenuItem("Night - Urban")) {}
                        ImGui::EndPopup();
                    }
                    ImGui::SameLine();

                    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
                }

                // Baking Tools
                {
                    if (ImGui::Button(ICON_MS_REFRESH "##BakeLighting", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Bake Lighting");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_SETTINGS "##BakeSettings", windowData->iconDefaultSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Bake Settings");
                }

                ImGui::PopStyleColor();
                ImGui::PopStyleVar(4); // Pop all pushed styles
            }
            ImGui::EndChild();
        }

        // Main content area
        if (ImGui::BeginChild("MainContent", ImVec2(-1, -1)))
        {
            // Left panel (fixed width)
            ImGui::BeginChild("LeftPanel", ImVec2(320.0f, 0), true);
            {
                // Filter/Search bar
                static char searchBuffer[64] = "";
                ImGui::SetNextItemWidth(-1);
                ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search lights...", searchBuffer, sizeof(searchBuffer));
                
                ImGui::Spacing();
                
                // Lights table
                if (ImGui::BeginTable("LightsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                {
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Vis", ImGuiTableColumnFlags_WidthFixed, 26.0f);
                    ImGui::TableSetupColumn("Shd", ImGuiTableColumnFlags_WidthFixed, 26.0f);
                    ImGui::TableSetupColumn("Int", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                    ImGui::TableHeadersRow();
                    
                    // Sample rows
                    for (int i = 0; i < 3; i++)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Selectable(("Light " + std::to_string(i + 1)).c_str(), false);
                        ImGui::TableNextColumn();
                        ImGui::Checkbox(("##vis" + std::to_string(i)).c_str(), &windowData->tempBool);
                        ImGui::TableNextColumn();
                        ImGui::Checkbox(("##shd" + std::to_string(i)).c_str(), &windowData->tempBool);
                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-1);
                        ImGui::DragFloat(("##int" + std::to_string(i)).c_str(), &windowData->tempFloat, 0.1f, 0.0f, 100.0f);
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Right panel (fills remaining space)
            ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
            {
                if (ImGui::BeginTabBar("LightingTabs"))
                {
                    if (ImGui::BeginTabItem("Properties"))
                    {
                        // Basic light properties
                        ImGui::Spacing();
                        
                        static const char* lightTypes[] = { "Directional", "Point", "Spot", "Area" };
                        static int currentType = 0;
                        ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes));
                        
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();

                        // Transform section
                        static float position[3] = {0.0f, 0.0f, 0.0f};
                        static float rotation[3] = {0.0f, 0.0f, 0.0f};
                        ImGui::Text("Transform");
                        ImGui::DragFloat3("Position", position, 0.1f);
                        ImGui::DragFloat3("Rotation", rotation, 1.0f);
                        
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();

                        // Light parameters
                        static float color[3] = {1.0f, 1.0f, 1.0f};
                        static float intensity = 100.0f;
                        static float range = 10.0f;
                        ImGui::Text("Light Parameters");
                        ImGui::ColorEdit3("Color", color);
                        ImGui::DragFloat("Intensity", &intensity, 1.0f, 0.0f, 1000.0f);
                        ImGui::DragFloat("Range", &range, 0.1f, 0.0f, 100.0f);

                        // Advanced parameters
                        if (ImGui::CollapsingHeader("Advanced"))
                        {
                            static float innerAngle = 0.0f;
                            static float outerAngle = 45.0f;
                            static float falloff = 1.0f;
                            
                            ImGui::DragFloat("Inner Angle", &innerAngle, 0.1f, 0.0f, outerAngle);
                            ImGui::DragFloat("Outer Angle", &outerAngle, 0.1f, innerAngle, 180.0f);
                            ImGui::DragFloat("Falloff", &falloff, 0.01f, 0.0f, 10.0f);
                        }

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Groups"))
                    {
                        // Group management
                        if (ImGui::Button("Create Group")) {}
                        ImGui::SameLine();
                        if (ImGui::Button("Delete Group")) {}
                        
                        ImGui::Spacing();
                        ImGui::Separator();
                        
                        // Group list with lights
                        if (ImGui::BeginChild("GroupList", ImVec2(-1, -1), true))
                        {
                            if (ImGui::TreeNode("Interior Lights"))
                            {
                                ImGui::Selectable("Kitchen Lights");
                                ImGui::Selectable("Living Room Lights");
                                ImGui::TreePop();
                            }
                            if (ImGui::TreeNode("Exterior Lights"))
                            {
                                ImGui::Selectable("Garden Lights");
                                ImGui::Selectable("Path Lights");
                                ImGui::TreePop();
                            }
                        }
                        ImGui::EndChild();
                        
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Linking"))
                    {
                        ImGui::BeginChild("LinkingSplitView", ImVec2(-1, -1), false);
                        {
                            // Left side: Light/Group selection
                            ImGui::BeginChild("LinkingLeft", ImVec2(200, -1), true);
                            {
                                if (ImGui::TreeNode("Lights"))
                                {
                                    ImGui::Selectable("Main Light");
                                    ImGui::Selectable("Fill Light");
                                    ImGui::TreePop();
                                }
                                if (ImGui::TreeNode("Light Groups"))
                                {
                                    ImGui::Selectable("Interior");
                                    ImGui::Selectable("Exterior");
                                    ImGui::TreePop();
                                }
                            }
                            ImGui::EndChild();

                            ImGui::SameLine();

                            // Right side: Link properties
                            ImGui::BeginChild("LinkingRight", ImVec2(0, -1), true);
                            {
                                ImGui::Text("Link Properties");
                                ImGui::Separator();
                                ImGui::Checkbox("Inherit Intensity", &windowData->tempBool);
                                ImGui::Checkbox("Inherit Color", &windowData->tempBool);
                                ImGui::Checkbox("Inherit Shadows", &windowData->tempBool);
                                ImGui::DragFloat("Intensity Multiplier", &windowData->tempFloat, 0.01f, 0.0f, 2.0f);
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Settings"))
                    {
                        ImGui::BeginChild("SettingsView", ImVec2(-1, -1), true);
                        {
                            if (ImGui::CollapsingHeader("Global Illumination"))
                            {
                                ImGui::Checkbox("Enable Global Illumination", &windowData->tempBool);
                                ImGui::DragFloat("Indirect Intensity", &windowData->tempFloat, 0.01f, 0.0f, 2.0f);
                                ImGui::DragFloat("Bounce Count", &windowData->tempFloat, 1.0f, 1.0f, 10.0f);
                            }

                            if (ImGui::CollapsingHeader("Shadow Settings"))
                            {
                                ImGui::Checkbox("Enable Cascaded Shadows", &windowData->tempBool);
                                ImGui::DragFloat("Shadow Distance", &windowData->tempFloat, 1.0f, 0.0f, 1000.0f);
                                ImGui::DragInt("Shadow Resolution", (int*)&windowData->tempFloat, 256, 512, 4096);
                            }

                            if (ImGui::CollapsingHeader("Performance"))
                            {
                                ImGui::Checkbox("Enable Light Culling", &windowData->tempBool);
                                ImGui::DragFloat("Light Fade Distance", &windowData->tempFloat, 1.0f, 0.0f, 1000.0f);
                                ImGui::DragInt("Max Active Lights", (int*)&windowData->tempFloat, 1, 1, 1000);
                            }
                        }
                        ImGui::EndChild();
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
