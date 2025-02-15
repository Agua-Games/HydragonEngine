/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "BottomToolbar.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowBottomToolbar(bool* p_open, HdEditorWindowData* windowData) 
{
    if (ImGui::Begin("Bottom Toolbar", p_open))
    {
        ImGui::Button("Test1");
    }
    ImGui::End();
}
} // namespace hdImgui