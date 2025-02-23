/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include "NodeGraphEditor.h"
#include "NodeGraphState.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"
#include <imgui_internal.h>
#include <string>

namespace hdImgui {

// Remove the NodeGraphState struct definition as it's now in the header

static NodeGraphState graphState;
static bool showConnectionPoints = true;  // Controls visibility of connection squares

// Forward declare internal helper functions
static void ShowNodeLibrary();
static void RenderNodeLibraryContent();
static void RenderGraphCanvasContent(HdEditorWindowData* windowData);
static void RenderMiniMapContent();
static void RenderTopToolbar(bool* p_open, HdEditorWindowData* windowData);
static void RenderRightSidebar();
static void RenderStatusBar();
static void RenderGraphCanvas(HdEditorWindowData* windowData);

// Forward declarations
static void RenderExampleNode(const char* title, ImVec2 pos, HdEditorWindowData* windowData);
static bool IsInputConnected(const char* inputName);

void ShowNodeGraphEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("Node Graph", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Graph")) {}
            if (ImGui::MenuItem("Open Graph")) {}
            if (ImGui::MenuItem("Save Graph")) {}
            if (ImGui::MenuItem("Save As...")) {}
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
            if (ImGui::MenuItem("Reset View")) {}
            if (ImGui::MenuItem("Frame All")) {}
            if (ImGui::MenuItem("Frame Selected")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Top Toolbar
    RenderTopToolbar(p_open, windowData);

    // Main Content Area
    ImGui::BeginChild("NodeGraphContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave space for status bar
    
    // Left panel for node library
    ImGui::BeginChild("NodeLibrary", ImVec2(200, 0), true);
    ShowNodeLibrary();
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Main graph canvas
    ImGui::BeginChild("GraphCanvas", ImVec2(0, 0), true);
    RenderGraphCanvas(windowData);
    ImGui::EndChild();

    ImGui::EndChild();
    
    // Status Bar
    RenderStatusBar();

    ImGui::End();
}

static void ShowNodeLibrary() 
{
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

void RenderGraphCanvas(HdEditorWindowData* windowData) 
{
    RenderGraphCanvasContent(windowData);
}

static void RenderGraphCanvasContent(HdEditorWindowData* windowData) 
{
    // Canvas setup and grid
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Style setup for nodes
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    
    // Track positions for all connections
    ImVec2 transformNodeOutputPos;
    ImVec2 materialNodeInputPos;
    ImVec2 materialNodeOutputPos;
    ImVec2 outputNodeInputPos;
    
    // Render nodes and calculate connection points
    {
        ImVec2 transformNodePos = ImVec2(100, 100);
        RenderExampleNode("Transform Node", transformNodePos, windowData);
        ImGuiWindow* transformWindow = ImGui::FindWindowByName("Transform Node");
        if (transformWindow) {
            transformNodeOutputPos = ImVec2(
                transformWindow->Pos.x + 250,  // Node width
                transformWindow->Pos.y + 30    // Approximate Y position of "Result" text
            );
        }
    }
    
    {
        ImVec2 materialNodePos = ImVec2(400, 150);
        RenderExampleNode("Material Node", materialNodePos, windowData);
        ImGuiWindow* materialWindow = ImGui::FindWindowByName("Material Node");
        if (materialWindow) {
            materialNodeInputPos = ImVec2(
                materialWindow->Pos.x,        // Left side of the node
                materialWindow->Pos.y + 50    // Approximate Y position of "Position" text
            );
            materialNodeOutputPos = ImVec2(
                materialWindow->Pos.x + 250,  // Node width
                materialWindow->Pos.y + 30    // Approximate Y position of "Result" text
            );
        }
    }
    
    {
        ImVec2 outputNodePos = ImVec2(700, 200);
        RenderExampleNode("Output Node", outputNodePos, windowData);
        ImGuiWindow* outputWindow = ImGui::FindWindowByName("Output Node");
        if (outputWindow) {
            outputNodeInputPos = ImVec2(
                outputWindow->Pos.x,        // Left side of the node
                outputWindow->Pos.y + 50    // Approximate Y position of "Position" text
            );
        }
    }

    // Draw connections
    const ImU32 lineColor = IM_COL32(127, 127, 127, 255);  // White with 0.3 alpha
    const float lineThickness = 2.0f;
    const float squareSize = 8.0f;  // Size of connection point squares

    // Draw connections with squares at endpoints
    if (transformNodeOutputPos.x != 0 && materialNodeInputPos.x != 0) {
        drawList->AddLine(transformNodeOutputPos, materialNodeInputPos, lineColor, lineThickness);
        
        if (showConnectionPoints) {
            // Output square
            drawList->AddRectFilled(
                ImVec2(transformNodeOutputPos.x - squareSize/2, transformNodeOutputPos.y - squareSize/2),
                ImVec2(transformNodeOutputPos.x + squareSize/2, transformNodeOutputPos.y + squareSize/2),
                lineColor
            );
            
            // Input square
            drawList->AddRectFilled(
                ImVec2(materialNodeInputPos.x - squareSize/2, materialNodeInputPos.y - squareSize/2),
                ImVec2(materialNodeInputPos.x + squareSize/2, materialNodeInputPos.y + squareSize/2),
                lineColor
            );
        }
    }

    if (materialNodeOutputPos.x != 0 && outputNodeInputPos.x != 0) {
        drawList->AddLine(materialNodeOutputPos, outputNodeInputPos, lineColor, lineThickness);
        
        if (showConnectionPoints) {
            // Output square
            drawList->AddRectFilled(
                ImVec2(materialNodeOutputPos.x - squareSize/2, materialNodeOutputPos.y - squareSize/2),
                ImVec2(materialNodeOutputPos.x + squareSize/2, materialNodeOutputPos.y + squareSize/2),
                lineColor
            );
            
            // Input square
            drawList->AddRectFilled(
                ImVec2(outputNodeInputPos.x - squareSize/2, outputNodeInputPos.y - squareSize/2),
                ImVec2(outputNodeInputPos.x + squareSize/2, outputNodeInputPos.y + squareSize/2),
                lineColor
            );
        }
    }

    ImGui::PopStyleVar(3);
}

static void RenderExampleNode(const char* title, ImVec2 pos, HdEditorWindowData* windowData)
{
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_FirstUseEver);
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    ImGuiWindowFlags nodeFlags = 
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoDocking;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.3f, 0.3f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.4f, 0.4f, 0.4f, 0.8f));
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 1.4f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1.4f));
    
    if (ImGui::Begin(title, nullptr, nodeFlags)) {
        float windowWidth = ImGui::GetContentRegionAvail().x;
        float inputColumnWidth = windowWidth * (2.0f/3.0f);  // 2/3 of width for inputs
        float outputColumnWidth = windowWidth * (1.0f/3.0f); // 1/3 of width for outputs

        // Left column (Inputs)
        ImGui::BeginGroup();
        {
            // Speed input
            ImGui::Text("Speed"); ImGui::SameLine();
            if (!IsInputConnected("Speed")) {
                ImGui::SetNextItemWidth(inputColumnWidth - ImGui::GetItemRectSize().x - ImGui::GetStyle().ItemSpacing.x - 10);
                float speed = 2.0f;
                if (ImGui::SliderFloat("##Speed", &speed, 0.0f, 10.0f)) {
                    // Handle value change
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Speed of the transformation\nType: float [0.0 - 10.0]");
                }
            }

            // Position input
            ImGui::Text("Position");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("World position\nType: Vector3");
            }
        }
        ImGui::EndGroup();

        // Right column (Outputs)
        ImGui::SameLine(inputColumnWidth);
        ImGui::BeginGroup();
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + outputColumnWidth - ImGui::CalcTextSize("Result").x - 4);
            ImGui::Text("Result");
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Transformed result\nType: Matrix4x4");
            }
        }
        ImGui::EndGroup();

        // Bottom toolbar with centered tiny icons
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20);
        
        float buttonsWidth = (8 * 3) + (8 * 2);
        float startX = (windowWidth - buttonsWidth) * 0.5f;
        ImGui::SetCursorPosX(startX);
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
        
        ImVec2 buttonSize(8, 8);
        
        if (ImGui::Button(ICON_MS_SETTINGS "##settings", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Node Settings");
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_DOCK "##dock", buttonSize)) {
            nodeFlags ^= ImGuiWindowFlags_NoDocking;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Docking");
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_DELETE "##delete", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Node");
        
        ImGui::PopStyleVar(2);
    }
    ImGui::End();
    
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(3);
}

// Placeholder function - to be implemented properly later
static bool IsInputConnected(const char* inputName) 
{
    return false;  // For now, always show widgets
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

static void RenderTopToolbar(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;

    const float toolbarHeight = 30.0f; // Fixed height for toolbar

    // Remove window rounding for the toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    // Create a child window with fixed height for the toolbar
    ImGui::BeginChild("NodeGraphToolbar", ImVec2(-1, toolbarHeight), true, 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse);

    // Match TopToolbar's style settings
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    
    // Style for buttons
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));
    
    // Ensure buttons start from the very top-left corner
    ImGui::SetCursorScreenPos(ImGui::GetWindowPos());

    // Left section - Transform tools
    {
        if (ImGui::Button(ICON_MS_OPEN_WITH "##Pan", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Pan Tool (H)");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_CROP_FREE "##Frame", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Frame Selected (F)");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_GRID_ON "##Grid", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Grid");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();
    }
    
    // Middle section - Node operations
    {
        if (ImGui::Button(ICON_MS_ADD_BOX "##AddNode", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Node (Tab)");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_CONTENT_CUT "##Cut", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cut Selected (Ctrl+X)");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_CONTENT_COPY "##Copy", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Copy Selected (Ctrl+C)");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_CONTENT_PASTE "##Paste", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Paste (Ctrl+V)");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(5,0)); ImGui::SameLine();
    }
    
    // Right section - Graph operations
    {
        if (ImGui::Button(ICON_MS_PLAY_ARROW "##Execute", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Execute Graph");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_STOP "##Stop", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop Execution");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_SAVE "##Save", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Quick Save (Ctrl+S)");
    }
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
    ImGui::EndChild();
    ImGui::PopStyleVar(); // Pop WindowRounding
}

static void RenderRightSidebar() 
{
    ImGui::BeginChild("RightSidebar", ImVec2(40, 0), true);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    
    // Center buttons horizontally in the sidebar
    float buttonX = (40.0f - 32.0f) * 0.5f;
    
    ImGui::SetCursorPosX(buttonX);
    if (ImGui::Button(ICON_MS_SETTINGS, ImVec2(32, 32))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Graph Settings");
    
    ImGui::SetCursorPosX(buttonX);
    if (ImGui::Button(ICON_MS_ANALYTICS, ImVec2(32, 32))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Graph Analysis");
    
    ImGui::SetCursorPosX(buttonX);
    if (ImGui::Button(ICON_MS_HISTORY, ImVec2(32, 32))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Graph History");
    
    ImGui::SetCursorPosX(buttonX);
    if (ImGui::Button(ICON_MS_BOOKMARK, ImVec2(32, 32))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Bookmarks");
    
    ImGui::SetCursorPosX(buttonX);
    if (ImGui::Button(ICON_MS_HELP, ImVec2(32, 32))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Graph Documentation");
    
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
}

static void RenderStatusBar() 
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
    ImGui::BeginChild("StatusBar", ImVec2(0, 24), true);
    
    // Performance stats
    ImGui::BeginGroup();
    ImGui::Text("FPS: %.1f", graphState.fps);
    ImGui::SameLine();
    ImGui::Text("| Mem: %.1f MB", graphState.graphMemoryUsage);
    ImGui::EndGroup();
    
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    
    // Graph stats
    ImGui::BeginGroup();
    ImGui::Text("Nodes: %d", graphState.nodeCount);
    ImGui::SameLine();
    ImGui::Text("| Connections: %d", graphState.connectionCount);
    ImGui::EndGroup();
    
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    
    // Warnings/Errors
    ImGui::BeginGroup();
    if (graphState.warningCount > 0)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), 
            ICON_MS_WARNING " %d", graphState.warningCount);
        ImGui::SameLine();
    }
    if (graphState.errorCount > 0)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), 
            ICON_MS_ERROR " %d", graphState.errorCount);
        ImGui::SameLine();
    }
    ImGui::EndGroup();
    
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    
    // Action buttons
    if (ImGui::Button(ICON_MS_AUTO_FIX_HIGH " Optimize")) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clean and organize graph");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_CACHED " Cache")) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Update node cache");
    
    // Graph status (right-aligned)
    float width = ImGui::CalcTextSize(graphState.isGraphDirty ? "Modified" : "Saved").x;
    ImGui::SameLine(ImGui::GetWindowWidth() - width - 10);
    if (graphState.isGraphDirty)
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Modified");
    else
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "Saved");
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

} // namespace hdImgui
