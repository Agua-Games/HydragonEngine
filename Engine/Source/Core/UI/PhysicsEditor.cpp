/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PhysicsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowPhysicsEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Physics Editor", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Physics Rig");
        }
        ImGui::End();
    }
} // namespace hdImgui