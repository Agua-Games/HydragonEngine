/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ConsoleEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowConsole(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Console", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Clear Console");
        ImGui::Button("Run Command");
    }
    ImGui::End();
}
} // namespace hdImgui