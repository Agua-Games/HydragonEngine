/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "IconsMaterialSymbols.h"
#include "CommandsPalette.h"
#include "hdImgui.h"

namespace hdImgui {

// Command structure to hold command data
struct Command {
    const char* name;
    const char* shortcut;
    const char* category;
    const char* icon;
    void (*callback)();  // Function pointer for command execution
};

static bool s_isSearchFocused = false;
static char s_searchBuffer[256] = "";
static int s_selectedIndex = 0;

void ShowCommandsPalette(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->GetCenter().x - 300, viewport->GetCenter().y - 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);

    if (ImGui::Begin("Commands", p_open, ImGuiWindowFlags_NoCollapse))
    {
        // Search bar with focus on window open
        if (!s_isSearchFocused) {
            ImGui::SetKeyboardFocusHere();
            s_isSearchFocused = true;
        }
        
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputTextWithHint("##CommandSearch", 
            ICON_MS_SEARCH " Type a command or search...", 
            s_searchBuffer, 
            IM_ARRAYSIZE(s_searchBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // Execute selected command on Enter
            // TODO: Execute command[s_selectedIndex]
            *p_open = false;  // Close palette after execution
        }

        // Category filters
        ImGui::BeginGroup();
        if (ImGui::Button(ICON_MS_ALL_INBOX " All##filter", ImVec2(80, 0))) {}
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_FILTER " File##filter", ImVec2(80, 0))) {}
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_EDIT " Edit##filter", ImVec2(80, 0))) {}
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_VIEW_IN_AR " View##filter", ImVec2(80, 0))) {}
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_SETTINGS " Tools##filter", ImVec2(80, 0))) {}
        ImGui::EndGroup();

        ImGui::Separator();

        // Commands list with categories
        if (ImGui::BeginChild("CommandsList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
        {
            // Example commands (in real implementation, these would be filtered based on search)
            const char* categories[] = { "File", "Edit", "View", "Tools" };
            
            for (const char* category : categories)
            {
                if (ImGui::CollapsingHeader(category, ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Indent();
                    
                    // Example command items
                    if (ImGui::Selectable("New Project##cmd", s_selectedIndex == 0))
                    {
                        s_selectedIndex = 0;
                    }
                    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
                    ImGui::TextDisabled("Ctrl+N");

                    if (ImGui::Selectable("Open Project##cmd", s_selectedIndex == 1))
                    {
                        s_selectedIndex = 1;
                    }
                    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
                    ImGui::TextDisabled("Ctrl+O");
                    
                    ImGui::Unindent();
                }
            }
        }
        ImGui::EndChild();

        // Status bar
        ImGui::Separator();
        ImGui::TextDisabled("Press Enter to execute command, Esc to cancel");
        
        // Handle keyboard navigation
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
            s_selectedIndex--;
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
            s_selectedIndex++;
        
        // Handle Escape to close
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            *p_open = false;
    }
    ImGui::End();

    // Reset focus flag when window closes
    if (!*p_open)
        s_isSearchFocused = false;
}

} // namespace hdImgui
