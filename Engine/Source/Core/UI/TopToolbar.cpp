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
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));  // Small spacing between items
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
            if (ImGui::Button(ICON_MS_MOVE "##Translate", windowData->iconDefaultSize)) { /* Handle Translate */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_ROTATE_RIGHT "##Rotate", windowData->iconDefaultSize)) { /* Handle Rotate */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_SCALE "##Scale", windowData->iconDefaultSize)) { /* Handle Scale */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_MULTIMODAL_HAND_EYE "##MultiTransform", windowData->iconDefaultSize)) { /* Handle Multi-Transform */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_DASHBOARD_CUSTOMIZE "##CustomGizmo", windowData->iconDefaultSize)) { /* Handle Custom Gizmo */ } ImGui::SameLine(); 
            ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();
        }
    
        // SECTION: Local/Global Orientation, Pivot Options
        {
            if (ImGui::Button(ICON_MS_PUBLIC "##Global", windowData->iconDefaultSize)) { /* Handle Global Orientation */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_LOCAL_ACTIVITY "##Local", windowData->iconDefaultSize)) { /* Handle Local Orientation */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_ADJUST "##Pivot", windowData->iconDefaultSize)) { /* Handle Object Pivot */ } ImGui::SameLine(); 
            if (ImGui::Button(ICON_MS_CENTER_FOCUS_STRONG "##Center", windowData->iconDefaultSize)) { /* Handle Object Center */ } ImGui::SameLine(); 
            ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();  
        }
    
        // SECTION: Selectable Category Toggles (Combo Box + Icons)
        ImGui::SetNextItemWidth(100.0f);
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
    
            // Icon buttons for each primitive type
            if (ImGui::Button(ICON_MS_CATEGORY "##Mesh", windowData->iconDefaultSize)) { /* Handle Mesh */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_LIGHT "##Light", windowData->iconDefaultSize)) { /* Handle Light */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_BLUR_ON "##Volume", windowData->iconDefaultSize)) { /* Handle Volume */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CAMERA "##Camera", windowData->iconDefaultSize)) { /* Handle Camera */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_VOLUME_UP "##Sound", windowData->iconDefaultSize)) { /* Handle Sound */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_BLUR_CIRCULAR "##Particle", windowData->iconDefaultSize)) { /* Handle Particle */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CROP_DIN "##Collider", windowData->iconDefaultSize)) { /* Handle Collider */ } ImGui::SameLine();
            if (ImGui::Button(ICON_MS_TERRAIN "##Terrain", windowData->iconDefaultSize)) { /* Handle Terrain */ } ImGui::SameLine();
            ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();  
        }
    
        // SECTION: Snapping (Translate, Rotate)
        {
            // Translate Snapping
            if (ImGui::Button(ICON_MS_GRID_ON "##Snap2DGrid", windowData->iconDefaultSize)) { /* Handle Snap to 2D Grid */ } ImGui::SameLine(); ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_GRID_3X3 "##Snap3DGrid", windowData->iconDefaultSize)) { /* Handle Snap to 3D Grid */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CONTROL_POINT "##SnapVertex", windowData->iconDefaultSize)) { /* Handle Snap to Vertex */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_TIMELINE "##SnapCurve", windowData->iconDefaultSize)) { /* Handle Snap to Curve */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_TEXTURE "##SnapSurface", windowData->iconDefaultSize)) { /* Handle Snap to Surface */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CROP_FREE "##SnapCorner", windowData->iconDefaultSize)) { /* Handle Snap to Corner */ } ImGui::SameLine(); 
            ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();  
        }
    
        // SECTION: Symmetry Types
        ImGui::SetNextItemWidth(100.0f);
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
            if (ImGui::Button(ICON_MS_GRAPH_1 "##NewGraph")) { /* Handle New Graph */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FOLDER_OPEN "##OpenGraph")) { /* Handle Open Graph */ } ImGui::SameLine(); 
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SAVE "##SaveGraph")) { /* Handle Save Graph */ } ImGui::SameLine(); 
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
                ICON_MS_GRAPH_2 "##SceneGraphEditor",
                ICON_MS_NETWORK_NODE "##NodeGraphEditor",
                ICON_MS_BOX "##AssetManager",
                ICON_MS_PIP_EXIT "##ChimeraPipeline",
                ICON_MS_FOLDER_OPEN "##FileExplorer",
                ICON_MS_TERMINAL "##Console",
                ICON_MS_CODE "##ScriptingEditor",
                ICON_MS_LIGHTBULB "##LightingEditor",
                ICON_MS_CAMERA_ALT "##Montage",
                ICON_MS_PHOTO_LIBRARY "##PhysicsEditor",
                ICON_MS_AUDIOTRACK "##AudioEditor"
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
        // Restore default style vars
        ImGui::PopStyleVar(4); // Pop WindowPadding, FramePadding, ItemSpacing, and FrameRounding (for buttons).
        ImGui::PopStyleColor(1); // Pop Button color)
    }
    ImGui::End(); // End the toolbar window
}

} //  namespace hdImgui
