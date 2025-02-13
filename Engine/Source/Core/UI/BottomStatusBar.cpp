/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "BottomStatusBar.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowBottomStatusBar(bool* p_open, HdEditorWindowData* windowData) 
    {
        if (ImGui::Begin("Status Bar", p_open))
        {
            ImGui::Button("FPS");
        }
        ImGui::End();
    }
} // namespace hdImgui