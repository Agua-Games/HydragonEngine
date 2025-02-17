/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "IconsFontAwesome6.h"
#include "IconsMaterialSymbols.h"

#include "TopToolbar.h"
#include "hdImgui.h"
#include "ResourceManager.h"

namespace hdImgui {
// === Layout variables ===
static float toolbarMaxHeight = 50.0f;

/**
 * @brief Displays the top toolbar in the UI.
 * @param p_open A pointer to a boolean variable. If not NULL, the top toolbar 
 *               will display a close button in the upper-right corner, and 
 *               clicking it will set the variable to false.
 */
void ShowTopToolbar(bool* p_open, HdEditorWindowData* windowData)
{
    // Set the toolbar to a horizontal strip layout
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar; // Hide tab when docked
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Compact padding for toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // Compact padding for toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 0));  // Small spacing between items
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();

    // Define min & max size constraints
    ImVec2 min_size(200, 30);  // Minimum width & height
    ImVec2 max_size(FLT_MAX, toolbarMaxHeight); // Max width is unlimited, height is fixed
    ImGui::SetNextWindowSizeConstraints(min_size, max_size);

    if (ImGui::Begin("Top Toolbar", p_open, ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);             // Eliminate button rounding
        ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);     // Get the default button color
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));     // Make button transparent

        ImGui::SetCursorScreenPos(ImGui::GetWindowPos());           // Reset cursor position to window origin (its top-left corner)

        // SECTION: SceneGraph (New, Open, Save)
        {
            if (ImGui::Button(ICON_MS_TRANSFORM "##NewScene", windowData->iconDefaultSize)) { /* Handle New Scene */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_LIGHTBULB "##OpenScene", windowData->iconDefaultSize)) { /* Handle Open Scene */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_TOOLS_WRENCH "##SaveScene", windowData->iconDefaultSize)) { /* Handle Save Scene */ } ImGui::SameLine();
            ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();  
        }
        // SECTION: Undo, Redo
        {
            if (ImGui::Button(ICON_MS_UNDO "##Undo", windowData->iconDefaultSize)) { /* Handle Undo */ } ImGui::SameLine();  
            if (ImGui::Button(ICON_MS_REDO "##Redo", windowData->iconDefaultSize)) { /* Handle Redo */ } ImGui::SameLine();     
        }
    
        // SECTION: Transform Gizmos (Translate, Rotate, Scale, Multi-Transform, Custom Gizmo)
        {
            if (ImGui::Button(ICON_MS_MOVE "##Translate", windowData->iconDefaultSize)) { /* Handle Translate */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_ROTATE_RIGHT "##Rotate", windowData->iconDefaultSize)) { /* Handle Rotate */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SCALE "##Scale", windowData->iconDefaultSize)) { /* Handle Scale */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_MULTIMODAL_HAND_EYE "##MultiTransform", windowData->iconDefaultSize)) { /* Handle Multi-Transform */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_DASHBOARD_CUSTOMIZE "##CustomGizmo", windowData->iconDefaultSize)) { /* Handle Custom Gizmo */ }
            ImGui::SameLine();
            ImGui::Separator();
        }
        #if 0
    
        // SECTION: Local/Global Orientation, Pivot Options
        {
            if (ImGui::Button(ICON_FA_GLOBE "##Global")) { /* Handle Global Orientation */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CUBE "##Local")) { /* Handle Local Orientation */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_DOT_CIRCLE "##Pivot")) { /* Handle Object Pivot */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ALIGN_CENTER "##Center")) { /* Handle Object Center */ }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: Selectable Category Toggles (Combo Box + Icons)
        {
            static const char* primitiveTypes[] = { "Mesh", "Light", "Volume", "Camera", "Sound", "Particle", "Collider", "Terrain" };
            static int selectedPrimitive = -1;
    
            if (ImGui::BeginCombo("##PrimitiveTypes", "Select Primitive")) {
                for (int i = 0; i < IM_ARRAYSIZE(primitiveTypes); i++) {
                    bool isSelected = (selectedPrimitive == i);
                    if (ImGui::Selectable(primitiveTypes[i], isSelected)) {
                        selectedPrimitive = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
    
            for (int i = 0; i < IM_ARRAYSIZE(primitiveTypes); i++) {
                std::string buttonLabel = ICON_FA_CUBE + std::to_string(i);
                if (ImGui::Button(buttonLabel.c_str())) { /* Handle Primitive Selection */ }
                ImGui::SameLine();
            }
            ImGui::Separator();
        }
    
        // SECTION: Snapping (Translate, Rotate)
        {
            // Translate Snapping
            if (ImGui::Button(ICON_FA_BORDER_ALL "##Snap2DGrid")) { /* Handle Snap to 2D Grid */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CUBE "##Snap3DGrid")) { /* Handle Snap to 3D Grid */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_DOT_CIRCLE "##SnapVertex")) { /* Handle Snap to Vertex */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_BEZIER_CURVE "##SnapCurve")) { /* Handle Snap to Curve */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_MESH_SURFACE "##SnapSurface")) { /* Handle Snap to Surface */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_CORNER "##SnapCorner")) { /* Handle Snap to Corner */ }
            ImGui::SameLine();
            ImGui::Separator();
    
            // Rotate Snapping
            static const char* snapAngles[] = { "1°", "5°", "10°", "15°", "20°", "25°" };
            static int selectedAngle = 0;
    
            if (ImGui::BeginCombo("##SnapAngles", snapAngles[selectedAngle])) {
                for (int i = 0; i < IM_ARRAYSIZE(snapAngles); i++) {
                    bool isSelected = (selectedAngle == i);
                    if (ImGui::Selectable(snapAngles[i], isSelected)) {
                        selectedAngle = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: Symmetry Types
        {
            static const char* symmetryTypes[] = { "None", "Object X", "Object Y", "Object Z", "World X", "World Y", "World Z", "Topology" };
            static int selectedSymmetry = 0;
    
            if (ImGui::BeginCombo("##SymmetryTypes", symmetryTypes[selectedSymmetry])) {
                for (int i = 0; i < IM_ARRAYSIZE(symmetryTypes); i++) {
                    bool isSelected = (selectedSymmetry == i);
                    if (ImGui::Selectable(symmetryTypes[i], isSelected)) {
                        selectedSymmetry = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: Commands Palette
        {
            static char commandBuffer[256] = "";
            ImGui::SetNextItemWidth(150);
            if (ImGui::InputText("##CommandPalette", commandBuffer, IM_ARRAYSIZE(commandBuffer))) {
                // Match typed words to existing commands
            }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: NodeGraph (New, Open, Save)
        {
            if (ImGui::Button(ICON_FA_PROJECT_DIAGRAM "##NewGraph")) { /* Handle New Graph */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FOLDER_OPEN "##OpenGraph")) { /* Handle Open Graph */ }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_SAVE "##SaveGraph")) { /* Handle Save Graph */ }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: Search Field for Scenegraph or Leaf
        {
            static char searchBuffer[256] = "";
            ImGui::SetNextItemWidth(200);
            if (ImGui::InputText("##SearchScenegraph", searchBuffer, IM_ARRAYSIZE(searchBuffer))) {
                // Handle wildcard search (e.g., "*_car.usd")
            }
            ImGui::SameLine();
            ImGui::Separator();
        }
    
        // SECTION: Commonly Used Editors
        {
            static const char* editorIcons[] = {
                ICON_FA_TREE "##SceneGraphEditor",
                ICON_FA_NETWORK_WIRED "##NodeGraphEditor",
                ICON_FA_BOXES "##AssetManager",
                ICON_FA_PIPE "##ChimeraPipeline",
                ICON_FA_FOLDER "##FileExplorer",
                ICON_FA_TERMINAL "##Console",
                ICON_FA_CODE "##ScriptingEditor",
                ICON_FA_LIGHTBULB "##LightingEditor",
                ICON_FA_FILM "##Montage",
                ICON_FA_ATOM "##PhysicsEditor",
                ICON_FA_VOLUME_UP "##AudioEditor"
            };
    
            for (const char* icon : editorIcons) {
                if (ImGui::Button(icon)) { /* Handle Editor Selection */ }
                ImGui::SameLine();
            }
            ImGui::Separator();
        }
    
        // SECTION: Layout Presets
        {
            static const char* layoutPresets[] = { "Workflow 1", "Workflow 2", "Workflow 3", "Workflow 4", "Workflow 5", "Restore Default", "Save Current", "Import Layout" };
            static int selectedLayout = 0;
    
            if (ImGui::BeginCombo("##LayoutPresets", layoutPresets[selectedLayout])) {
                for (int i = 0; i < IM_ARRAYSIZE(layoutPresets); i++) {
                    bool isSelected = (selectedLayout == i);
                    if (ImGui::Selectable(layoutPresets[i], isSelected)) {
                        selectedLayout = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
        }
        #endif
        // Restore default style vars
        ImGui::PopStyleVar(4); // Pop WindowPadding, FramePadding, ItemSpacing, and FrameRounding (for buttons).
        ImGui::PopStyleColor(1); // Pop Button color)
    }
    ImGui::End(); // End the toolbar window
}

} //  namespace hdImgui
