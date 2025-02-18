/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "IconsMaterialSymbols.h"
#include "MacrosEditor.h"
#include "hdImgui.h"

namespace hdImgui {

static bool s_isRecording = false;
static bool s_isPaused = false;
static char s_macroName[256] = "";
static int s_selectedCommandIndex = -1;

void ShowMacrosEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Macros##Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Main toolbar with recording controls
        ImGui::BeginGroup();
        {
            if (!s_isRecording)
            {
                if (ImGui::Button(ICON_MS_FIBER_MANUAL_RECORD " Start Recording##macro"))
                {
                    s_isRecording = true;
                    // TODO: Start command logging
                }
            }
            else
            {
                // Record/Pause controls
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                if (ImGui::Button(s_isPaused ? ICON_MS_PLAY_CIRCLE " Resume##macro" : ICON_MS_PAUSE " Pause##macro"))
                {
                    s_isPaused = !s_isPaused;
                    // TODO: Pause/Resume command logging
                }
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                if (ImGui::Button(ICON_MS_STOP " Stop##macro"))
                {
                    s_isRecording = false;
                    s_isPaused = false;
                    // TODO: Stop command logging, create macro node
                }
            }
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            if (ImGui::Button(ICON_MS_SAVE " Save##macro")) {}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_FOLDER_OPEN " Load##macro")) {}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_PLAY_ARROW " Run##macro")) {}
        }
        ImGui::EndGroup();

        ImGui::Separator();

        // Main content area with splitter
        ImGui::Columns(2, "MacroEditorColumns", true);
        
        // Left panel: Macro Library
        {
            ImGui::Text("Macro Library");
            ImGui::Separator();

            if (ImGui::BeginChild("##MacroLibrary", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
            {
                // Temporary Macros (Session)
                if (ImGui::CollapsingHeader("Temporary Macros", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::PushID("TempMacros");
                    ImGui::Indent();
                    
                    // Example temporary macros
                    if (ImGui::Selectable("Quick Selection + Mask##temp")) {}
                    if (ImGui::Selectable("Export Selected Assets##temp")) {}
                    
                    ImGui::Unindent();
                    ImGui::PopID();
                }

                // Saved Macros (Persistent)
                if (ImGui::CollapsingHeader("Saved Macros", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::PushID("SavedMacros");
                    ImGui::Indent();
                    
                    // Categories
                    if (ImGui::TreeNode("Asset Processing"))
                    {
                        if (ImGui::Selectable("Batch Texture Resize##saved")) {}
                        if (ImGui::Selectable("Generate LODs##saved")) {}
                        ImGui::TreePop();
                    }
                    
                    if (ImGui::TreeNode("Scene Setup"))
                    {
                        if (ImGui::Selectable("Setup Basic Level##saved")) {}
                        if (ImGui::Selectable("Import & Configure Nav##saved")) {}
                        ImGui::TreePop();
                    }
                    
                    ImGui::Unindent();
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();
        }
        
        ImGui::NextColumn();

        // Right panel: Command Sequence / Editor
        {
            ImGui::Text(s_isRecording ? "Recording Commands" : "Macro Editor");
            ImGui::Separator();

            if (ImGui::BeginChild("##MacroEditor", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
            {
                if (s_isRecording && s_isPaused)
                {
                    // Show recorded commands with reordering capabilities
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.5f));
                    
                    // Example recorded commands
                    if (ImGui::Selectable("1. Select All Objects##cmd", s_selectedCommandIndex == 0))
                        s_selectedCommandIndex = 0;
                        
                    if (ImGui::Selectable("2. Apply Transform##cmd", s_selectedCommandIndex == 1))
                        s_selectedCommandIndex = 1;
                        
                    if (ImGui::Selectable("3. Export Selection##cmd", s_selectedCommandIndex == 2))
                        s_selectedCommandIndex = 2;
                    
                    ImGui::PopStyleColor();

                    // Command reordering buttons
                    if (s_selectedCommandIndex >= 0)
                    {
                        ImGui::Separator();
                        if (ImGui::Button(ICON_MS_ARROW_UPWARD " Move Up##cmd") && s_selectedCommandIndex > 0)
                        {
                            // TODO: Reorder commands
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(ICON_MS_ARROW_DOWNWARD " Move Down##cmd"))
                        {
                            // TODO: Reorder commands
                        }
                        ImGui::SameLine();
                        if (ImGui::Button(ICON_MS_DELETE " Delete##cmd"))
                        {
                            // TODO: Delete command
                        }
                    }
                }
                else if (!s_isRecording)
                {
                    // Macro properties and node configuration
                    ImGui::InputText("Macro Name##input", s_macroName, IM_ARRAYSIZE(s_macroName));
                    
                    ImGui::Separator();
                    ImGui::Text("Node Configuration");
                    
                    // Node input/output configuration
                    if (ImGui::TreeNode("Inputs##node"))
                    {
                        ImGui::Checkbox("Accept Selection##node", nullptr);
                        ImGui::Checkbox("Accept Parameters##node", nullptr);
                        ImGui::TreePop();
                    }
                    
                    if (ImGui::TreeNode("Outputs##node"))
                    {
                        ImGui::Checkbox("Return Modified Selection##node", nullptr);
                        ImGui::Checkbox("Return Success Status##node", nullptr);
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::EndChild();
        }
        
        ImGui::Columns(1);
    }
    ImGui::End();
}

} // namespace hdImgui
