/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PropertyEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowPropertyEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Properties", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Load Template");
        }
        ImGui::End();
    }
} // namespace hdImgui