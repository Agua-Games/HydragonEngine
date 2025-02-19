/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>

#include "PerformanceScalabilityEditor.h"
#include "hdImgui.h"

namespace hdImgui {

static const char* QualityLevels[] = { "Low", "Medium", "High", "Maximum" };
static int CurrentPreset = 0;

void ShowPerformanceScalabilityEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Performance", p_open, 
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Config", "Ctrl+N")) {}
                if (ImGui::MenuItem("Open Config...", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Import...")) {}
                if (ImGui::MenuItem("Export...")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Close")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Presets"))
            {
                if (ImGui::MenuItem("Low (Performance)")) {}
                if (ImGui::MenuItem("Medium (Balanced)")) {}
                if (ImGui::MenuItem("High (Quality)")) {}
                if (ImGui::MenuItem("Maximum (Ultra)")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Save Current as Preset...")) {}
                if (ImGui::MenuItem("Import Preset...")) {}
                if (ImGui::MenuItem("Export Preset...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Analyze Performance Impact")) {}
                if (ImGui::MenuItem("Benchmark Current Settings")) {}
                if (ImGui::MenuItem("Generate Performance Report")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Reset to Defaults")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Main Content
        ImGui::Spacing();
        
        // Quick Preset Selection
        const char* presets[] = { "Custom", "Low (Performance)", "Medium (Balanced)", "High (Quality)", "Maximum (Ultra)" };
        ImGui::Text("Quick Preset:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("##QuickPreset", &CurrentPreset, presets, IM_ARRAYSIZE(presets));
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Settings Table
        static ImGuiTableFlags flags = 
            ImGuiTableFlags_Borders | 
            ImGuiTableFlags_RowBg | 
            ImGuiTableFlags_SizingFixedFit;

        if (ImGui::BeginTable("ScalabilitySettings", 5, flags))
        {
            ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Low", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Medium", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("High", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Maximum", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            // Define categories and their current values
            struct ScalabilityCategory {
                const char* name;
                int current_value;
            };

            static ScalabilityCategory categories[] = {
                {"View Distance", 0},
                {"Anti-Aliasing", 0},
                {"Post Processing", 0},
                {"Shadows", 0},
                {"Textures", 0},
                {"Effects", 0},
                {"Foliage", 0},
                {"Shading", 0},
                {"Mesh Quality", 0},
                {"Volumetrics", 0}
            };

            for (auto& category : categories)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(category.name);

                for (int quality = 0; quality < 4; quality++)
                {
                    ImGui::TableNextColumn();
                    ImGui::PushID(&category + quality);
                    if (ImGui::Selectable(QualityLevels[quality], category.current_value == quality, 
                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap))
                    {
                        category.current_value = quality;
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Bottom Controls
        if (ImGui::Button("Apply Changes"))
        {
            // Apply logic here
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset All"))
        {
            // Reset logic here
        }
        
        ImGui::SameLine();
        ImGui::Spacing();
        ImGui::SameLine();
        
        // Performance Impact Indicator
        ImGui::TextDisabled("Estimated Performance Impact:");
        ImGui::SameLine();
        ImGui::ProgressBar(0.7f, ImVec2(200, 0), "Medium");
        
        // Memory Usage
        ImGui::SameLine();
        ImGui::TextDisabled("Estimated VRAM Usage:");
        ImGui::SameLine();
        ImGui::ProgressBar(0.5f, ImVec2(200, 0), "2.4 GB");
    }
    ImGui::End();
}
} // namespace hdImgui
