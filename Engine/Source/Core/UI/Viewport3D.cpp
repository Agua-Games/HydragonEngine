/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "Viewport3D.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowViewport3D(bool* p_open, HdEditorWindowData* windowData) 
{
    if (ImGui::Begin("Viewport 3D", p_open, ImGuiWindowFlags_MenuBar))
    {
        // TODO
    }
    ImGui::End();
}
} // namespace hdImgui