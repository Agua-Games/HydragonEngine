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
    if (ImGui::Begin("Lighting Editor", p_open))
    {
        // Top toolbar
        if (ImGui::BeginChild("Toolbar", ImVec2(-1, 30), true))
        {
            if (ImGui::Button(ICON_MS_ADD)) {}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CONTENT_COPY)) {}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_DELETE)) {}
            ImGui::SameLine();
            ImGui::Separator();
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SAVE)) {}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_FOLDER_OPEN)) {}
        }
        ImGui::EndChild();

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
                        // Split view for linking
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
                                    ImGui::Selectable("Interior Group");
                                    ImGui::Selectable("Exterior Group");
                                    ImGui::TreePop();
                                }
                            }
                            ImGui::EndChild();

                            ImGui::SameLine();

                            // Right side: Scene/Asset linking
                            ImGui::BeginChild("LinkingRight", ImVec2(0, -1), true);
                            {
                                // Tabs for different link targets
                                if (ImGui::BeginTabBar("LinkTargets"))
                                {
                                    if (ImGui::BeginTabItem("SceneGraph"))
                                    {
                                        if (ImGui::TreeNode("Scene"))
                                        {
                                            ImGui::Selectable("Building_01");
                                            if (ImGui::TreeNode("Interior"))
                                            {
                                                ImGui::Selectable("Room_01");
                                                ImGui::Selectable("Room_02");
                                                ImGui::TreePop();
                                            }
                                            ImGui::TreePop();
                                        }
                                        ImGui::EndTabItem();
                                    }
                                    if (ImGui::BeginTabItem("Assets"))
                                    {
                                        if (ImGui::TreeNode("Models"))
                                        {
                                            ImGui::Selectable("Character_01");
                                            ImGui::Selectable("Vehicle_01");
                                            ImGui::TreePop();
                                        }
                                        ImGui::EndTabItem();
                                    }
                                    ImGui::EndTabBar();
                                }
                            }
                            ImGui::EndChild();
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
