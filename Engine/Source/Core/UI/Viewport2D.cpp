/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "Viewport2D.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowViewport2D(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Viewport 2D", p_open, ImGuiWindowFlags_MenuBar))
        {
            // TODO
        }
        ImGui::End();
    }
} // namespace hdImgui