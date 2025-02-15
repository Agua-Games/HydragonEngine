/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "FileExplorer.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowFileExplorer(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("File Explorer", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("New...");
        ImGui::Button("Open...");
        ImGui::Button("Open As...");
        ImGui::Button("Open Recent");
        ImGui::Button("Close");
        ImGui::Button("Close All");
        ImGui::Button("Save");
        ImGui::Button("Save As...");
        ImGui::Button("Import");
        ImGui::Button("Export");
        ImGui::Button("Automate");
        ImGui::Button("File Info");
        ImGui::Button("Bookmarks");
    }
    ImGui::End();
}
} // namespace hdImgui