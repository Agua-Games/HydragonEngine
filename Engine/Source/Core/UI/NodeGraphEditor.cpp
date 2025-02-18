/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include "NodeGraphEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"
#include <imgui_internal.h>
#include <string>

namespace hdImgui {

// Node graph state
struct NodeGraphState {
    float zoomLevel = 1.0f;
    ImVec2 panOffset = ImVec2(0.0f, 0.0f);
    bool isDragging = false;
    bool isConnecting = false;
    int selectedNode = -1;
    ImVec2 lastMousePos;
    bool showMiniMap = true;
    bool showNodeLibrary = true;
    float nodeLibraryWidth = 250.0f;
};

static NodeGraphState graphState;

// Forward declare internal helper functions
static void RenderNodeLibraryContent();
static void RenderGraphCanvasContent();
static void RenderMiniMapContent();

void ShowNodeGraphEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);

    if (ImGui::Begin("NodeGraph", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Top Toolbar with essential controls
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Graph", "Ctrl+N")) {}
                if (ImGui::MenuItem("Open Graph", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save Graph", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Export Graph")) {}
                if (ImGui::MenuItem("Import Graph")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Node Library", "Tab", &graphState.showNodeLibrary);
                ImGui::MenuItem("Mini Map", "M", &graphState.showMiniMap);
                ImGui::Separator();
                if (ImGui::MenuItem("Frame All", "A")) {}
                if (ImGui::MenuItem("Frame Selected", "F")) {}
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Quick Access Toolbar
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
            
            if (ImGui::Button(ICON_MS_ADD_CIRCLE)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Node (Tab)");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_PLAY_CIRCLE)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Execute Graph");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SAVE)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Quick Save");

            ImGui::SameLine();
            ImGui::Checkbox("Auto", &windowData->nodeGraphEditor_autoCompile);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto-Compile");
            
            ImGui::PopStyleVar();
        }

        // Main Content Area
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        
        // Node Library Panel (Left Side)
        if (graphState.showNodeLibrary)
        {
            ImGui::BeginChild("NodeLibrary", ImVec2(graphState.nodeLibraryWidth, 0), true);
            ShowNodeLibrary();
            ImGui::EndChild();
            
            // Resizer
            ImGui::SameLine();
            ImGui::Button("##NodeLibraryResizer", ImVec2(4, -1));
            if (ImGui::IsItemActive())
            {
                graphState.nodeLibraryWidth += ImGui::GetIO().MouseDelta.x;
                graphState.nodeLibraryWidth = ImClamp(graphState.nodeLibraryWidth, 150.0f, 400.0f);
            }
            ImGui::SameLine();
        }

        // Graph Canvas
        ImGui::BeginChild("GraphCanvas", ImVec2(0, 0), true);
        RenderGraphCanvas();
        ImGui::EndChild();

        // Mini Map Overlay
        if (graphState.showMiniMap)
        {
            RenderMiniMap();
        }

        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void ShowNodeLibrary() 
{
    // Search bar
    static char searchBuffer[64] = "";
    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search Nodes...", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    ImGui::PopItemWidth();
    ImGui::Separator();

    RenderNodeLibraryContent();
}

static void RenderNodeLibraryContent() 
{
    // Node Categories
    if (ImGui::CollapsingHeader("Flow Control", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Selectable("Branch");
        ImGui::Selectable("Sequence");
        ImGui::Selectable("For Each");
    }

    if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Selectable("Get Variable");
        ImGui::Selectable("Set Variable");
        ImGui::Selectable("Array");
    }

    if (ImGui::CollapsingHeader("Math", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Selectable("Add");
        ImGui::Selectable("Multiply");
        ImGui::Selectable("Vector3");
    }

    if (ImGui::CollapsingHeader("Graphs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Selectable("Sub Graph");
        ImGui::Selectable("Graph Input");
        ImGui::Selectable("Graph Output");
    }
}

void RenderGraphCanvas() 
{
    RenderGraphCanvasContent();
}

static void RenderGraphCanvasContent() 
{
    // Canvas setup
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Background grid
    const float GRID_STEP = 64.0f * graphState.zoomLevel;
    for (float x = fmodf(graphState.panOffset.x, GRID_STEP); x < canvasSize.x; x += GRID_STEP)
    {
        drawList->AddLine(
            ImVec2(canvasPos.x + x, canvasPos.y),
            ImVec2(canvasPos.x + x, canvasPos.y + canvasSize.y),
            IM_COL32(50, 50, 50, 100)
        );
    }
    for (float y = fmodf(graphState.panOffset.y, GRID_STEP); y < canvasSize.y; y += GRID_STEP)
    {
        drawList->AddLine(
            ImVec2(canvasPos.x, canvasPos.y + y),
            ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + y),
            IM_COL32(50, 50, 50, 100)
        );
    }

    // Handle input
    if (ImGui::IsWindowHovered())
    {
        // Pan
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            graphState.panOffset.x += ImGui::GetIO().MouseDelta.x;
            graphState.panOffset.y += ImGui::GetIO().MouseDelta.y;
        }

        // Zoom
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f)
        {
            const float zoomSpeed = 0.1f;
            graphState.zoomLevel += wheel * zoomSpeed;
            graphState.zoomLevel = ImClamp(graphState.zoomLevel, 0.1f, 2.0f);
        }
    }

    // TODO: Render actual nodes and connections here
}

void RenderMiniMap() 
{
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 210,
               ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 160),
        ImGuiCond_Always
    );
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_Always);
    
    if (ImGui::Begin("##MiniMap", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar))
    {
        RenderMiniMapContent();
    }
    ImGui::End();
}

static void RenderMiniMapContent() 
{
    // TODO: Render miniature version of the graph
    ImGui::Text("Mini Map");
}

} // namespace hdImgui
