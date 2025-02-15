/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "VolumeEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowVolumeEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Volume", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import VDB Volume");
        ImGui::Button("New VDB Volume");
        ImGui::Button("Save Volume");
        ImGui::Button("Edit Volume");
        ImGui::Button("Load Volume");
        ImGui::Button("Procedural VDB Nodes");      // overlay, context menus
        ImGui::Button("Volumetric VDB Tools");          // overlay, context menus
        ImGui::Button("Volume Settings");
    }
    ImGui::End();
}
} // namespace hdImgui