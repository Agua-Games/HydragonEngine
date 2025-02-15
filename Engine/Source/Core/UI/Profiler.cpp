/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "Profiler.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowProfiler(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Profiler", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Checkbox("Recording Enabled", &windowData->profilerRecordingEnabled);
        ImGui::Button("Start Profiler");
        ImGui::Button("Pause Profiler");
        ImGui::Button("Stop Profiler");
        ImGui::Button("Clear Profiler");
        ImGui::Button("Export Profiler");
        ImGui::Button("Share Profiler");
        ImGui::Button("Save Profiler");
        ImGui::Button("Export Profiling Session");
        ImGui::Button("Profiler Settings");
    }
    ImGui::End();
}
} // namespace hdImgui