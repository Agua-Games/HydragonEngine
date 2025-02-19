/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <memory>
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

    // Toolbar
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));
        
        ImGui::BeginGroup();
        {
            ImVec2 buttonSize(32, 32);
            
            if (ImGui::Button(ICON_MS_ADD_BOX "##New", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Widget");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_CONTENT_COPY "##Duplicate", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Duplicate");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_DELETE "##Delete", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete");
            
            ImGui::SameLine(0, 20);
            
            for (const auto& widget : state.quickWidgets) {
                if (ImGui::Button((widget.icon + std::string("##") + widget.name).c_str(), buttonSize)) {}
                if (ImGui::IsItemHovered()) ImGui::SetTooltip(widget.name);
                ImGui::SameLine();
            }
        }
        ImGui::EndGroup();
        
        ImGui::PopStyleVar(2);
        ImGui::Separator();
    }

    // Main content area
    ImGui::BeginChild("UIEditorContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    {
        // Left panel - Widget Library
        ImGui::BeginChild("WidgetLibrary", ImVec2(200, 0), true);
        ShowWidgetLibrary();
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Center panel - UI Preview
        ImGui::BeginChild("UIPreviewArea", ImVec2(-200, 0), true);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            
            if (ImGui::Button(ICON_MS_PREVIEW "##Preview")) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Preview");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_REFRESH "##Refresh")) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Refresh Preview");
            
            ImGui::SameLine();
            ImGui::Text("UI Preview");
            
            ImGui::PopStyleVar();
            ImGui::Separator();

            ImGui::BeginChild("PreviewContent", ImVec2(0, 0), true);
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
                "UI Preview Area\n\n"
                "Note: Use the NodeGraphEditor (available in separate tab)\n"
                "to set up node chains and logic for your UI elements.");
            ImGui::EndChild();
        }
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Right panel - Properties
        ImGui::BeginChild("Properties", ImVec2(200, 0), true);
        ShowProperties();
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
