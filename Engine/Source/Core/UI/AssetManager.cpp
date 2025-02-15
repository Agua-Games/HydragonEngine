/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "AssetManager.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowAssetManager(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Asset Manager", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Asset");
        ImGui::Button("New Asset");
        ImGui::Button("Export Asset");
        ImGui::Button("Share Asset");
        ImGui::Button("Import Library");
        ImGui::Button("New Library");
        ImGui::Button("Library Settings");
        ImGui::Button("Add to Library");
        ImGui::Button("Remove from Library");
        ImGui::Button("Rename Library");
        ImGui::Button("Delete Library");
        ImGui::Button("Export Library");
        ImGui::Button("Share Library");
    }
    ImGui::End();
}
} // namespace hdImgui