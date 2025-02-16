/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 *  TODO:
 *  - Force the status bar height to be shorter.
 */
#pragma once
#include <string>
#include <stdio.h>
#include <imgui.h>

#include "BottomStatusBar.h"
#include "hdImgui.h"

namespace hdImgui {

// === Layout variables ===
static float statusBarHeight = 9.0f;

/**
 * @brief Displays the bottom status bar in the UI.
 * @param p_open A pointer to a boolean variable. If not NULL, the bottom status bar 
 *               will display a close button in the upper-right corner, and 
 *               clicking it will set the variable to false.
 */
void ShowBottomStatusBar(bool* p_open, HdEditorWindowData* windowData) 
{
    // Set the toolbar to a horizontal strip layout
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoResize; // Hide tab when docked
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // Compact padding for toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 5)); // Compact padding for toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));  // Small spacing between items

    // Define min & max size constraints
    ImVec2 min_size(FLT_MAX, statusBarHeight);  // Minimum width & height
    ImVec2 max_size(FLT_MAX, statusBarHeight); // Max width is unlimited, height is fixed
    ImGui::SetNextWindowSizeConstraints(min_size, max_size);

    if (ImGui::Begin("Status Bar", p_open, ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        // === FPS ===
        ImGui::Text("Application average: %.1f FPS | %.3f ms/frame", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        ImGui::SameLine();
        // === Drawing Metrics ===
        // *(Refactor the lines below later - it was just copied from imgui's example code)
        //Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
        ImGui::SameLine();
        //Text("%d visible windows, %d current allocations", io.MetricsRenderWindows, g.DebugAllocInfo.TotalAllocCount - g.DebugAllocInfo.TotalFreeCount);
        ImGui::SameLine();
        
        // Restore default style vars
        ImGui::PopStyleVar(3); // Pop FramePadding and ItemSpacing
        
        if (ImGui::Button("Toggle Sleep")) {}
    }
    ImGui::End();
}

} // namespace hdImgui