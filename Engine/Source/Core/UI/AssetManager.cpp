/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>
#include "IconsMaterialSymbols.h"

#include "AssetManager.h"
#include "hdImgui.h"

namespace hdImgui {

// Forward declaration of helper function
const char* GetAssetTypeIcon(int index);

// Static variables for persistent state
static float s_thumbnailScale = 1.0f;

void ShowAssetManager(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Asset Manager", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(ICON_MS_ADD " New Asset Library")) {}
                if (ImGui::MenuItem(ICON_MS_IMPORT_EXPORT " Import Assets...")) {}
                if (ImGui::MenuItem(ICON_MS_CLOUD_DOWNLOAD " Export Selected...")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_SETTINGS " Preferences...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem(ICON_MS_GRID_VIEW " Thumbnails View")) {}
                if (ImGui::MenuItem(ICON_MS_VIEW_LIST " List View")) {}
                if (ImGui::MenuItem(ICON_MS_DETAILS " Details View")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_REFRESH " Refresh")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Top Toolbar with common actions and view controls
        {
            // Left side controls
            if (ImGui::Button(ICON_MS_ADD "##NewAssetBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Asset");
            ImGui::SameLine();

            if (ImGui::Button(ICON_MS_CLOUD_UPLOAD "##ImportBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Import Assets");
            ImGui::SameLine();

            if (ImGui::Button(ICON_MS_CLOUD_DOWNLOAD "##ExportBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Export Selected");
            ImGui::SameLine();

            ImGui::Dummy(ImVec2(10, 0)); ImGui::SameLine();

            // Search bar
            static char searchBuffer[256] = "";
            ImGui::SetNextItemWidth(200);
            if (ImGui::InputTextWithHint("##SearchAssets", ICON_MS_SEARCH " Search assets...", searchBuffer, IM_ARRAYSIZE(searchBuffer))) {}
            ImGui::SameLine();

            // Right side controls
            ImGui::SetNextItemWidth(120);
            if (ImGui::SliderFloat("##ScaleSlider", &s_thumbnailScale, 0.5f, 2.0f, "Scale: %.1f")) {}
            ImGui::SameLine();

            // View mode toggles
            if (ImGui::Button(ICON_MS_GRID_VIEW "##GridViewBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Thumbnails View");
            ImGui::SameLine();

            if (ImGui::Button(ICON_MS_VIEW_LIST "##ListViewBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("List View");
            ImGui::SameLine();

            if (ImGui::Button(ICON_MS_DETAILS "##DetailsViewBtn", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Details View");
        }

        // Main content area
        ImGui::Columns(2, "AssetManagerColumns", true);
        ImGui::SetColumnWidth(0, 250); // Left panel width

        // Left panel - Directory Tree
        {
            // Directory tree with icons
            if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Assets", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Models", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Characters", ImGuiTreeNodeFlags_Leaf))
                        ImGui::TreePop();
                    if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Props", ImGuiTreeNodeFlags_Leaf))
                        ImGui::TreePop();
                    if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Environments", ImGuiTreeNodeFlags_Leaf))
                        ImGui::TreePop();
                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Materials", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    if (ImGui::TreeNodeEx(ICON_MS_FOLDER " PBR", ImGuiTreeNodeFlags_Leaf))
                        ImGui::TreePop();
                    if (ImGui::TreeNodeEx(ICON_MS_FOLDER " Shaders", ImGuiTreeNodeFlags_Leaf))
                        ImGui::TreePop();
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::NextColumn();

        // Right panel - Asset Grid/List View
        {
            // Example asset grid with thumbnails
            float padding = 8.0f;
            float thumbnailSize = 128.0f * s_thumbnailScale;
            float cellSize = thumbnailSize + padding * 2;
            
            float panelWidth = ImGui::GetContentRegionAvail().x;
            int columns = (int)(panelWidth / cellSize);
            if (columns < 1) columns = 1;

            if (ImGui::BeginChild("AssetGrid"))
            {
                // Example assets with rounded thumbnails
                for (int i = 0; i < 20; i++)
                {
                    ImGui::BeginGroup();
                    
                    // Thumbnail with rounded corners
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                    ImGui::PushID(i); // Push unique ID based on index
                    ImGui::Button(GetAssetTypeIcon(i), ImVec2(thumbnailSize, thumbnailSize));
                    ImGui::PopID();
                    ImGui::PopStyleVar();
                    
                    // Asset name
                    char name[32];
                    snprintf(name, sizeof(name), "Asset %d##asset_%d", i, i);  // Add unique ID to label
                    float textWidth = ImGui::CalcTextSize(name).x;
                    float textX = (thumbnailSize - textWidth) * 0.5f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textX);
                    ImGui::Text("%s", name);
                    
                    ImGui::EndGroup();
                    
                    if ((i + 1) % columns != 0)
                        ImGui::SameLine();
                }
                ImGui::EndChild();
            }
        }

        ImGui::Columns(1);
    }
    ImGui::End();
}

// Helper function implementation
const char* GetAssetTypeIcon(int index)
{
    switch (index % 6)
    {
        case 0: return ICON_MS_MODEL_TRAINING;  // 3D Model
        case 1: return ICON_MS_TEXTURE;         // Texture
        case 2: return ICON_MS_WAVES;           // Audio
        case 3: return ICON_MS_ANIMATION;       // Animation
        case 4: return ICON_MS_WB_SHADE;        // Shader
        case 5: return ICON_MS_GLOBE;           // Material
        default: return ICON_MS_HELP;
    }
}

} // namespace hdImgui
