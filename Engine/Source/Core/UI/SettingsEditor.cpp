/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "SettingsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowSettingsEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Settings", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("New Settings");
        ImGui::Button("Load Settings");
        ImGui::Button("Save Settings");
    }
    ImGui::End();
}
} // namespace hdImgui