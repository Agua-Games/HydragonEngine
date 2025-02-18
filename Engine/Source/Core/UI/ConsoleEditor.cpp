/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "IconsMaterialSymbols.h"
#include "ConsoleEditor.h"
#include "hdImgui.h"

namespace hdImgui {

// Console state
static char s_inputBuffer[2048] = "";
static ImVector<char*> s_items;
static ImVector<char*> s_history;
static int s_historyPos = -1;    // -1: new line, 0..history.Size-1 browsing history
static ImGuiTextFilter s_filter;
static bool s_autoScroll = true;
static bool s_scrollToBottom = false;
static int s_selectedItem = -1;

// Console colors
static ImVec4 INFO_COLOR    = ImVec4(0.45f, 0.75f, 0.45f, 1.00f);
static ImVec4 WARNING_COLOR = ImVec4(0.85f, 0.85f, 0.45f, 1.00f);
static ImVec4 ERROR_COLOR   = ImVec4(0.85f, 0.45f, 0.45f, 1.00f);
static ImVec4 DEBUG_COLOR   = ImVec4(0.45f, 0.65f, 0.85f, 1.00f);

void ShowConsole(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    // Console window flags
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    
    if (ImGui::Begin("Console", p_open, window_flags))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::MenuItem("Auto-scroll", nullptr, &s_autoScroll);
                ImGui::MenuItem("Show Timestamps", nullptr, &windowData->showConsoleTimestamps);
                ImGui::MenuItem("Show Categories", nullptr, &windowData->showConsoleCategories);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Filter"))
            {
                ImGui::MenuItem("Info Messages", nullptr, &windowData->showInfoMessages);
                ImGui::MenuItem("Warnings", nullptr, &windowData->showWarnings);
                ImGui::MenuItem("Errors", nullptr, &windowData->showErrors);
                ImGui::MenuItem("Debug Messages", nullptr, &windowData->showDebugMessages);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Actions"))
            {
                if (ImGui::MenuItem("Clear Console", "Ctrl+L")) 
                {
                    // TODO: Clear console items
                }
                if (ImGui::MenuItem("Export Log", "Ctrl+S")) 
                {
                    // TODO: Export console to file
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Search bar and controls
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            
            // Left side: Search and filters
            ImGui::BeginGroup();
            {
                s_filter.Draw(ICON_MS_SEARCH " Filter", 180.0f);
                
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.75f, 0.45f, 0.40f));
                ImGui::Button(ICON_MS_INFO);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show Info Messages");
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.85f, 0.45f, 0.40f));
                ImGui::Button(ICON_MS_WARNING);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show Warnings");
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.45f, 0.45f, 0.40f));
                ImGui::Button(ICON_MS_ERROR);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show Errors");
                ImGui::PopStyleColor();
            }
            ImGui::EndGroup();
            
            // Right side: Actions
            ImGui::SameLine(ImGui::GetWindowWidth() - 160);
            ImGui::BeginGroup();
            {
                if (ImGui::Button(ICON_MS_CLEAR_ALL " Clear"))
                {
                    // TODO: Clear console items
                }
                ImGui::SameLine();
                if (ImGui::Button(ICON_MS_SAVE " Export"))
                {
                    // TODO: Export console to file
                }
            }
            ImGui::EndGroup();
            
            ImGui::PopStyleVar();
        }

        ImGui::Separator();

        // Console output
        {
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, 
                            ImGuiWindowFlags_HorizontalScrollbar);
            
            // Context menu for selected item
            if (ImGui::BeginPopupContextWindow())
            {
                if (s_selectedItem != -1)
                {
                    if (ImGui::MenuItem("Copy"))
                    {
                        // TODO: Copy selected item to clipboard
                    }
                    if (ImGui::MenuItem("Copy All"))
                    {
                        // TODO: Copy all visible items to clipboard
                    }
                }
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            
            // Example log items with different types
            ImGui::PushStyleColor(ImGuiCol_Text, INFO_COLOR);
            ImGui::TextUnformatted("[13:45:28][Info] Engine initialization completed");
            ImGui::PopStyleColor();
            
            ImGui::PushStyleColor(ImGuiCol_Text, WARNING_COLOR);
            ImGui::TextUnformatted("[13:45:29][Warning] Asset 'texture_01.png' not found in cache");
            ImGui::PopStyleColor();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ERROR_COLOR);
            ImGui::TextUnformatted("[13:45:30][Error] Failed to load shader 'post_process.hlsl'");
            ImGui::PopStyleColor();
            
            ImGui::PushStyleColor(ImGuiCol_Text, DEBUG_COLOR);
            ImGui::TextUnformatted("[13:45:31][Debug] Memory usage: 1.2GB");
            ImGui::PopStyleColor();
            
            ImGui::PopStyleVar();

            if (s_scrollToBottom || (s_autoScroll && ImGui::GetScrollY() >= 
                ImGui::GetScrollMaxY())) ImGui::SetScrollHereY(1.0f);
            s_scrollToBottom = false;
            
            ImGui::EndChild();
        }

        // Command input
        {
            ImGui::Separator();
            bool reclaimFocus = false;
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            ImGui::PushItemWidth(-1);
            
            if (ImGui::InputText("##ConsoleInput", s_inputBuffer, IM_ARRAYSIZE(s_inputBuffer),
                               ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | 
                               ImGuiInputTextFlags_CallbackHistory))
            {
                char* input_end = s_inputBuffer + strlen(s_inputBuffer);
                while (input_end > s_inputBuffer && input_end[-1] == ' ') input_end--;
                *input_end = 0;
                
                if (s_inputBuffer[0])
                {
                    // TODO: Execute command
                    // TODO: Add to history
                }
                
                strcpy(s_inputBuffer, "");
                reclaimFocus = true;
            }
            
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();

            // Auto-focus on window apparition
            ImGui::SetItemDefaultFocus();
            if (reclaimFocus)
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }
    }
    ImGui::End();
}

} // namespace hdImgui
