/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>  // for std::clamp
#include "UIEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"
#include "VulkanBackend.h"  // Add necessary graphics backend

namespace hdImgui {

// Forward declarations
static void ShowWidgetLibrary();
static void ShowProperties();

// State management
static struct UIEditorState {
    char searchBuffer[256] = "";
    bool showWidgetPreview = true;
    std::string selectedCategory = "All";
    bool isInitialized = false;
    
    // Properties panel state
    struct {
        float position[2] = {0.0f, 0.0f};
        float size[2] = {100.0f, 100.0f};
        float rotation = 0.0f;
        float backgroundColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float textColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        float borderWidth = 1.0f;
        float cornerRadius = 0.0f;
        bool isVisible = true;
        bool isInteractive = true;
        bool clipContent = false;
    } properties;

    // Quick access widgets
    struct QuickWidget {
        const char* name;
        const char* icon;
        const char* category;
    };
    
    std::vector<QuickWidget> quickWidgets = {
        {"Button", ICON_MS_SMART_BUTTON, "Basic"},
        {"Text", ICON_MS_TEXT_FIELDS, "Basic"},
        {"Image", ICON_MS_IMAGE, "Basic"},
        {"Panel", ICON_MS_GRID_VIEW, "Containers"},
        {"ComboBox", ICON_MS_ARROW_DROP_DOWN_CIRCLE, "Input"},
        {"TreeView", ICON_MS_ACCOUNT_TREE, "Navigation"}
    };
} state;

// Initialize UI Editor
bool InitializeUIEditor() {
    if (state.isInitialized) {
        return true;
    }

    // Add any necessary initialization here
    state.isInitialized = true;
    return true;
}

// Cleanup UI Editor
void CleanupUIEditor() {
    if (!state.isInitialized) {
        return;
    }

    // Add cleanup code here
    state.isInitialized = false;
}

static void ShowWidgetLibrary()
{
    if (!state.isInitialized) return;

    ImGui::Text("Widget Library");
    ImGui::Separator();
    
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    // Categories
    if (ImGui::TreeNodeEx("Basic", nodeFlags))
    {
        if (ImGui::Selectable("Button")) {}
        if (ImGui::Selectable("Text")) {}
        if (ImGui::Selectable("Image")) {}
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Containers", nodeFlags))
    {
        if (ImGui::Selectable("Panel")) {}
        if (ImGui::Selectable("ScrollView")) {}
        if (ImGui::Selectable("TabView")) {}
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Input", nodeFlags))
    {
        if (ImGui::Selectable("TextInput")) {}
        if (ImGui::Selectable("ComboBox")) {}
        if (ImGui::Selectable("Checkbox")) {}
        ImGui::TreePop();
    }
}

static void ShowProperties()
{
    if (!state.isInitialized) return;

    ImGui::Text("Properties");
    ImGui::Separator();
    
    ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
    
    // Layout section
    if (ImGui::CollapsingHeader("Layout", headerFlags))
    {
        ImGui::BeginGroup();
        ImGui::DragFloat2("Position", state.properties.position, 1.0f);
        ImGui::DragFloat2("Size", state.properties.size, 1.0f);
        ImGui::DragFloat("Rotation", &state.properties.rotation, 1.0f);
        ImGui::EndGroup();
    }
    
    // Style section
    if (ImGui::CollapsingHeader("Style", headerFlags))
    {
        ImGui::BeginGroup();
        ImGui::ColorEdit4("Background", state.properties.backgroundColor);
        ImGui::ColorEdit4("Text Color", state.properties.textColor);
        ImGui::DragFloat("Border Width", &state.properties.borderWidth, 0.1f);
        ImGui::DragFloat("Corner Radius", &state.properties.cornerRadius, 0.1f);
        ImGui::EndGroup();
    }
    
    // Behavior section
    if (ImGui::CollapsingHeader("Behavior", headerFlags))
    {
        ImGui::BeginGroup();
        ImGui::Checkbox("Visible", &state.properties.isVisible);
        ImGui::Checkbox("Interactive", &state.properties.isInteractive);
        ImGui::Checkbox("Clip Content", &state.properties.clipContent);
        ImGui::EndGroup();
    }
}

void ShowUIEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!state.isInitialized) {
        if (!InitializeUIEditor()) {
            return;
        }
    }

    if (!windowData) {
        return;
    }

    // Store panel widths in static variables
    static float widgetLibraryWidth = 200.0f;
    static float propertiesWidth = 200.0f;
    const float minPanelWidth = 150.0f;
    const float maxPanelWidth = 400.0f;

    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    if (!ImGui::Begin("UI Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New UI", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Import")) {}
            if (ImGui::MenuItem("Export")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Copy Style", "Ctrl+Shift+C")) {}
            if (ImGui::MenuItem("Paste Style", "Ctrl+Shift+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Widget Preview", nullptr, &state.showWidgetPreview);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar - Improved layout based on TopToolbar.cpp
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
        
        ImGui::BeginChild("UIEditorToolbar", ImVec2(-1, 40), false);
        {
            const float toolbarHeight = 32.0f;
            const ImVec2 buttonSize(32, toolbarHeight);
            const float groupSpacing = 20.0f;
            
            // Left-aligned buttons group
            ImGui::BeginGroup();
            {
                if (ImGui::Button(ICON_MS_ADD_BOX "##New", buttonSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Widget");
                
                ImGui::SameLine();
                if (ImGui::Button(ICON_MS_CONTENT_COPY "##Duplicate", buttonSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Duplicate");
                
                ImGui::SameLine();
                if (ImGui::Button(ICON_MS_DELETE "##Delete", buttonSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete");
            }
            ImGui::EndGroup();
            
            ImGui::SameLine(0, groupSpacing);
            
            // Quick widgets group
            ImGui::BeginGroup();
            {
                for (size_t i = 0; i < state.quickWidgets.size(); ++i) {
                    const auto& widget = state.quickWidgets[i];
                    if (ImGui::Button((widget.icon + std::string("##") + widget.name).c_str(), buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip(widget.name);
                    
                    if (i < state.quickWidgets.size() - 1)
                        ImGui::SameLine();
                }
            }
            ImGui::EndGroup();
        }
        ImGui::EndChild();
        
        ImGui::PopStyleVar(3);
        ImGui::Separator();
    }

    // Main content area
    ImGui::BeginChild("UIEditorContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    {
        // Left panel - Widget Library
        ImGui::BeginChild("WidgetLibrary", ImVec2(widgetLibraryWidth, 0), true);
        ShowWidgetLibrary();
        ImGui::EndChild();

        ImGui::SameLine();
        
        // Left panel resizer
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
        ImGui::Button("##WidgetLibraryResizer", ImVec2(4.0f, -1));
        if (ImGui::IsItemActive())
        {
            widgetLibraryWidth += ImGui::GetIO().MouseDelta.x;
            widgetLibraryWidth = std::clamp(widgetLibraryWidth, minPanelWidth, maxPanelWidth);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        
        // Center panel - UI Preview
        float previewWidth = ImGui::GetContentRegionAvail().x - propertiesWidth - 4.0f;
        ImGui::BeginChild("UIPreviewArea", ImVec2(previewWidth, 0), true);
        {
            // Preview toolbar - using TopToolbar.cpp styling
            const float toolbarHeight = 30.0f;
            
            // Match TopToolbar's style settings
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            
            ImGui::BeginChild("PreviewToolbar", ImVec2(-1, toolbarHeight), true, 
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            
            // Style for buttons
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));
            
            // Set cursor position to window origin
            ImGui::SetCursorScreenPos(ImGui::GetWindowPos());
            
            // Preview Controls Section
            {
                if (ImGui::Button(ICON_MS_PREVIEW "##Preview", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Preview");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_REFRESH "##Refresh", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Refresh Preview");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_GRID_ON "##ToggleGrid", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Grid");
                ImGui::SameLine();
                
                ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
            }
            
            // View Controls Section
            {
                if (ImGui::Button(ICON_MS_ZOOM_IN "##ZoomIn", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Zoom In");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ZOOM_OUT "##ZoomOut", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Zoom Out");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_FIT_SCREEN "##FitToScreen", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Fit to Screen");
                ImGui::SameLine();
                
                ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
            }
            
            // Alignment Section
            {
                if (ImGui::Button(ICON_MS_ALIGN_HORIZONTAL_LEFT "##AlignLeft", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Left");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ALIGN_HORIZONTAL_CENTER "##AlignCenter", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Center");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ALIGN_HORIZONTAL_RIGHT "##AlignRight", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Right");
                ImGui::SameLine();
                
                ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ALIGN_VERTICAL_TOP "##AlignTop", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Top");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ALIGN_VERTICAL_CENTER "##AlignMiddle", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Middle");
                ImGui::SameLine();
                
                if (ImGui::Button(ICON_MS_ALIGN_VERTICAL_BOTTOM "##AlignBottom", windowData->iconDefaultSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Align Bottom");
            }
            
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(4); // Pop all style variables
            
            ImGui::EndChild(); // End PreviewToolbar
            
            ImGui::Separator();

            // Preview content
            ImGui::BeginChild("PreviewContent", ImVec2(0, 0), true);
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
                "UI Preview Area\n\n"
                "Note: Use the NodeGraphEditor (available in separate tab)\n"
                "to set up node chains and logic for your UI elements.");
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        
        // Properties panel resizer
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
        ImGui::Button("##PropertiesResizer", ImVec2(4.0f, -1));
        if (ImGui::IsItemActive())
        {
            propertiesWidth -= ImGui::GetIO().MouseDelta.x;
            propertiesWidth = std::clamp(propertiesWidth, minPanelWidth, maxPanelWidth);
        }
        if (ImGui::IsItemHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        
        // Right panel - Properties
        ImGui::BeginChild("Properties", ImVec2(propertiesWidth, 0), true);
        ShowProperties();
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
