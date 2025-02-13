/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PluginEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowPluginEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Plugin", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Plugin");
            ImGui::Button("Add Plugin");
            ImGui::Button("Save Plugin");
            ImGui::Button("Load Plugin");
            ImGui::Button("Unload Plugin");
            ImGui::Button("Reload Plugin");
            ImGui::Button("Export Plugin");
            ImGui::Button("Share Plugin");
            ImGui::Button("Remove Plugin");
            ImGui::Button("Plugin Settings");
        }  
        ImGui::End();
    }
} // namespace hdImgui