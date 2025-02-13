/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ExtensionsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowExtensionsEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Extensions", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Add Extension");
            ImGui::Button("Save Extension");
            ImGui::Button("Load Extension");
            ImGui::Button("Unload Extension");
            ImGui::Button("Reload Extension");
            ImGui::Button("Export Extension");
            ImGui::Button("Share Extension");
            ImGui::Button("Remove Extension");
            ImGui::Button("Extension Settings");
        }   
        ImGui::End();
    }
} // namespace hdImgui