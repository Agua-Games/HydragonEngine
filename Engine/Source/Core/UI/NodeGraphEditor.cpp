/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * The Node Graph Editor uses imgui and the imgui-node-editor (extension) library.
 * The implementation will be based on the example provided in the imgui-node-editor repository.
 * 
 * ARCHITECTURAL NOTE:
 * This editor follows the engine's node-graph centric architecture:
 * - All functionality is represented as nodes in the graph
 * - The UI is a visualization layer for the underlying node structure
 * - Sub-editors are consolidated views of related nodes and their properties
 * - The Properties window is the central point for editing node parameters
 *
 *  TODO:
 *  - Properly separate imgui-node-editor's initialization and destruction (move these to hdImgui) from render/update (implemented here).
 *  - Cleanup and refactor the whole file, after each session of bringing code snippets from the examples.
 *      - Get rid of unused structs, variables etc.
 *      - Organize the code into logical sections and functions.
 *      - Put the file into proper order, well-structured, cohesive.
 *      - Gradually move it closer to the blueprints-example.cpp file, as this file has the most complete implementation.
 */
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "IconsMaterialSymbols.h"
#include <imgui.h>
#include "imgui_node_editor.h"
namespace nodeEd = ax::NodeEditor;

#include "NodeGraphEditor.h"
#include "NodeGraphState.h"
#include "hdImgui.h"
//#include "HD_Node.h"                  // For node integration - next (architecture) steps
#include "PropertyEditor.h"             // For property integration

namespace hdImgui {

// Add this at the top with other static variables
static nodeEd::EditorContext* g_NodeEditorContext = nullptr;

// === For next (architecture) steps ===
// Forward declarations for sub-editor integration
//void NotifyNodeSelected(HD_Node* node);
//void UpdateSubEditors(HD_Node* selectedNode);
// === end of next steps ===

static NodeGraphState graphState;  // Instance of our state struct

struct ViewportState {
    ImVec2 viewPosition = ImVec2(0.0f, 0.0f);  // Camera position in world space
    float zoom = 1.0f;  // For future use
} viewport;

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

struct NodeData {
    ImVec2 worldPos;
    bool isDragging;
};

struct NodeConnection {
    nodeEd::PinId outputPinId;
    nodeEd::PinId inputPinId;
    nodeEd::NodeId outputNodeId;
    nodeEd::NodeId inputNodeId;
};

struct LinkInfo
{
    nodeEd::LinkId Id;
    nodeEd::PinId InputId;
    nodeEd::PinId OutputId;
};

static bool g_FirstFrame = true;
static ImVector<LinkInfo> g_Links;
static int g_NextId = 1; // Used to generate unique IDs
static bool showConnectionPoints = true;  // Controls visibility of connection squares
static std::unordered_map<std::string, NodeData> nodePositions;

void InitializeNodeGraphEditor(HdEditorWindowData* windowData) {
    // Only initialize if not already done
    if (g_NodeEditorContext == nullptr) {
        // Initialize node graph state
        graphState = NodeGraphState();

        nodeEd::Config config;
        config.SettingsFile = "NodeEditorSettings.json";        // Optional: save layout to file

        // Make sure navigation is enabled
        config.NavigateButtonIndex = ImGuiMouseButton_Middle;  // Middle mouse button for panning
        config.DragButtonIndex = ImGuiMouseButton_Left;        // Left mouse button for dragging nodes
        g_NodeEditorContext = nodeEd::CreateEditor(&config);
    }
}

void ShutdownNodeGraphEditor() {
    if (g_NodeEditorContext) {
        nodeEd::DestroyEditor(g_NodeEditorContext);
        g_NodeEditorContext = nullptr;
    }
}

static ImVec2 WorldToScreen(const ImVec2& worldPos, const ImVec2& canvasOrigin) {
    // 1. Transform from world space to view space (subtract camera position)
    ImVec2 viewSpace = ImVec2(
        worldPos.x - viewport.viewPosition.x,
        worldPos.y - viewport.viewPosition.y
    );
    
    // 2. Transform to screen space (add canvas origin)
    return ImVec2(
        canvasOrigin.x + viewSpace.x,
        canvasOrigin.y + viewSpace.y
    );
}

// Forward declare internal helper functions
static void RenderNodeLibrary();
static void RenderNodeLibraryContent();
static void RenderGraphCanvasContent(HdEditorWindowData* windowData);
static void RenderMiniMapContent();
static void RenderTopToolbar(bool* p_open, HdEditorWindowData* windowData);
static void RenderRightSidebar();
static void RenderStatusBar();
static void RenderGraphCanvas(HdEditorWindowData* windowData);

// Forward declarations
static void RenderExampleNode();
static bool IsInputConnected(const char* inputName);

void ShowNodeGraphEditor(bool* p_open, HdEditorWindowData* windowData) 
{   
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("Node Graph", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::PopStyleVar();
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
            if (ImGui::MenuItem("Straight Links")) {}
            if (ImGui::MenuItem("Reset Panning")) {
                nodeEd::NavigateToContent();
            }
            if (ImGui::MenuItem("Reset Zoom")) {
                nodeEd::NavigateToContent();  // This will reset both panning and zoom
            }
            if (ImGui::MenuItem("Frame All")) {}
            if (ImGui::MenuItem("Frame Selected")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Remove spacing and rounding for main layout elements
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // Top Toolbar
    RenderTopToolbar(p_open, windowData);

    // Main Content Area - removed spacing before status bar
    ImGui::BeginChild("NodeGraphContent", ImVec2(0, -ImGui::GetFrameHeight())); 
    
    // Add resizable splitter for Node Library
    static float libraryWidth = 200.0f;
    const float minLibraryWidth = 100.0f;
    const float maxLibraryWidth = 400.0f;

    // Left panel for node library - with inner padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::BeginChild("NodeLibrary", ImVec2(libraryWidth, 0), true);
    RenderNodeLibrary();
    ImGui::EndChild();
    ImGui::PopStyleVar();

    // Add resizable splitter
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.3f));
    ImGui::Button("##splitter", ImVec2(4.0f, -1));
    if (ImGui::IsItemActive())
    {
        libraryWidth += ImGui::GetIO().MouseDelta.x;
        libraryWidth = std::clamp(libraryWidth, minLibraryWidth, maxLibraryWidth);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    
    // Main graph canvas
    ImGui::BeginChild("GraphCanvas", ImVec2(0, 0), true);
    RenderGraphCanvas(windowData);
    ImGui::EndChild();

    ImGui::EndChild(); // End NodeGraphContent

    // Status Bar
    RenderStatusBar();

    // Pop the style modifications for main layout
    ImGui::PopStyleVar(5); // Pop WindowPadding, WindowRounding, ChildRounding, ItemSpacing

    ImGui::End(); // End Node Graph
}

static void RenderNodeLibrary() 
{
    // Add padding inside the library window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    // Push styles for internal content (keeping rounded corners for elements inside)
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));

    // Search bar
    static char searchBuffer[64] = "";
    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search Nodes...", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    ImGui::PopItemWidth();
    ImGui::Separator();

    // Library content
    RenderNodeLibraryContent();

    // Make sure to pop all style vars
    ImGui::PopStyleVar(3);  // Pop all three style vars
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
}

void RenderGraphCanvas(HdEditorWindowData* windowData)
{
    // Check if editor context exists
    if (g_NodeEditorContext == nullptr) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Node Editor not initialized!");
        return;
    }

    // Set current editor context
    nodeEd::SetCurrentEditor(g_NodeEditorContext);
    
    // Begin the node editor canvas
    nodeEd::Begin("Node Editor", ImVec2(0.0f, 0.0f));
    
    // === FIRST NODE ===
    static nodeEd::NodeId nodeId1 = 1;
    static nodeEd::PinId inputPinId1 = 2;
    static nodeEd::PinId outputPinId1 = 3;
    
    // === SECOND NODE ===
    static nodeEd::NodeId nodeId2 = 4;
    static nodeEd::PinId inputPinId2 = 5;
    static nodeEd::PinId outputPinId2 = 6;
    
    // Set node positions only once
    if (g_FirstFrame)
    {
        nodeEd::SetNodePosition(nodeId1, ImVec2(200, 200));
        nodeEd::SetNodePosition(nodeId2, ImVec2(500, 200));
        g_FirstFrame = false;
    }
    
    // Begin first node
    nodeEd::BeginNode(nodeId1);
    
    // Node title
    ImGui::TextUnformatted("Node A");
    
    // Add some content to make the node bigger and easier to grab
    ImGui::Dummy(ImVec2(100, 10));
    
    // Input pin
    nodeEd::BeginPin(inputPinId1, nodeEd::PinKind::Input);
    ImGui::TextUnformatted("-> Input");
    nodeEd::EndPin();
    
    ImGui::SameLine();
    
    // Output pin
    nodeEd::BeginPin(outputPinId1, nodeEd::PinKind::Output);
    ImGui::TextUnformatted("Output ->");
    nodeEd::EndPin();
    
    nodeEd::EndNode();
    
    // Begin second node
    nodeEd::BeginNode(nodeId2);
    
    // Node title
    ImGui::TextUnformatted("Node B");
    
    // Add some content to make the node bigger and easier to grab
    ImGui::Dummy(ImVec2(100, 10));
    
    // Input pin
    nodeEd::BeginPin(inputPinId2, nodeEd::PinKind::Input);
    ImGui::TextUnformatted("-> Input");
    nodeEd::EndPin();
    
    ImGui::SameLine();
    
    // Output pin
    nodeEd::BeginPin(outputPinId2, nodeEd::PinKind::Output);
    ImGui::TextUnformatted("Output ->");
    nodeEd::EndPin();
    
    nodeEd::EndNode();
    
    // Draw existing links
    for (auto& link : g_Links)
    {
        nodeEd::Link(link.Id, link.InputId, link.OutputId);
    }
    
    // Handle interactions for creating links
    if (nodeEd::BeginCreate())
    {
        nodeEd::PinId startPinId, endPinId;
        if (nodeEd::QueryNewLink(&startPinId, &endPinId))
        {
            // Check if connection is valid (output to input)
            if (startPinId && endPinId)
            {
                // Since we can't directly query pin kind, we'll use our knowledge of how we set up the pins
                // We know that pins 2 and 5 are inputs, and pins 3 and 6 are outputs
                bool startPinIsInput = (startPinId == inputPinId1 || startPinId == inputPinId2);
                bool endPinIsInput = (endPinId == inputPinId1 || endPinId == inputPinId2);
                
                nodeEd::PinId inputPinId = 0, outputPinId = 0;
                
                if (startPinIsInput && !endPinIsInput)
                {
                    inputPinId = startPinId;
                    outputPinId = endPinId;
                }
                else if (!startPinIsInput && endPinIsInput)
                {
                    inputPinId = endPinId;
                    outputPinId = startPinId;
                }
                else
                {
                    // Invalid connection (input to input or output to output)
                    nodeEd::RejectNewItem(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::SetTooltip("Cannot connect pins of the same type!");
                    inputPinId = outputPinId = 0;
                }
                
                if (inputPinId && outputPinId)
                {
                    // Check if this connection already exists
                    bool connectionExists = false;
                    for (auto& link : g_Links)
                    {
                        if (link.InputId == inputPinId && link.OutputId == outputPinId)
                        {
                            connectionExists = true;
                            break;
                        }
                    }
                    
                    if (connectionExists)
                    {
                        // Connection already exists
                        nodeEd::RejectNewItem(ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
                        ImGui::SetTooltip("Connection already exists!");
                    }
                    else if (nodeEd::AcceptNewItem())
                    {
                        // Add a new link
                        LinkInfo link;
                        link.Id = nodeEd::LinkId(g_NextId++);
                        link.InputId = inputPinId;
                        link.OutputId = outputPinId;
                        g_Links.push_back(link);
                    }
                }
            }
        }
        nodeEd::EndCreate();
    }
    
    // Handle node/link deletion
    if (nodeEd::BeginDelete())
    {
        // Handle link deletion
        nodeEd::LinkId linkId;
        while (nodeEd::QueryDeletedLink(&linkId))
        {
            if (nodeEd::AcceptDeletedItem())
            {
                // Remove the link with this ID
                for (int i = 0; i < g_Links.size(); ++i)
                {
                    if (g_Links[i].Id == linkId)
                    {
                        g_Links.erase(g_Links.begin() + i);
                        break;
                    }
                }
            }
        }
        
        // Handle node deletion
        nodeEd::NodeId nodeId;
        while (nodeEd::QueryDeletedNode(&nodeId))
        {
            if (nodeEd::AcceptDeletedItem())
            {
                // In a real application, you would delete the node here
                // For this example, we'll just log it
                ImGui::SetTooltip("Node %d deleted", nodeId.Get());
                
                // Remove all links connected to this node
                for (int i = g_Links.size() - 1; i >= 0; --i)
                {
                    // This is a simplified check - in a real app, you'd need to know which pins belong to which nodes
                    // For this example, we're just showing the concept
                    if (nodeId.Get() == 1 && (g_Links[i].InputId == inputPinId1 || g_Links[i].OutputId == outputPinId1))
                    {
                        g_Links.erase(g_Links.begin() + i);
                    }
                    else if (nodeId.Get() == 4 && (g_Links[i].InputId == inputPinId2 || g_Links[i].OutputId == outputPinId2))
                    {
                        g_Links.erase(g_Links.begin() + i);
                    }
                }
            }
        }
        
        nodeEd::EndDelete();
    }
    
    // End the node editor canvas
    nodeEd::End();
    
    // Reset the current editor to nullptr (good practice)
    nodeEd::SetCurrentEditor(nullptr);
}

void RenderGraphCanvasContent(HdEditorWindowData* windowData) 
{ 

}

static void RenderExampleNode()
{
    // This function is intentionally empty for now
    // We'll implement it step by step after we confirm the canvas works
}

// Placeholder function - to be implemented properly later
static bool IsInputConnected(const char* inputName) 
{
    return false;  // For now, always show widgets
}

void RenderMiniMap() 
{
    // Position and size the minimap window
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

    const float toolbarHeight = 35.0f; // Fixed height for toolbar

    // Make toolbar have no rounding
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f); 

    // Remove window rounding for the toolbar
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, toolbarHeight), ImVec2(FLT_MAX, toolbarHeight));    // Force fixed height

    // Create a child window with fixed height for the toolbar
    // Note: BeginChild doesn't take a p_open parameter, so we don't pass it here
    if (ImGui::BeginChild("NodeGraphToolbar", ImVec2(-1, toolbarHeight), true, 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse))
    {
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
            
            // Frame Selected button
            if (ImGui::Button(ICON_MS_CROP_FREE "##Frame", windowData->iconDefaultSize)) {
                // Use NavigateToSelection to frame selected nodes
                nodeEd::NavigateToSelection(true); 
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Frame Selected (F)");
            ImGui::SameLine();
            
            // Reset View button
            if (ImGui::Button(ICON_MS_RESTART_ALT "##ResetView", windowData->iconDefaultSize)) {
                // Use NavigateToContent to reset view
                nodeEd::NavigateToContent();
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reset View");
            ImGui::SameLine();
            
            // Add Grid toggle using opacity
            static bool showGrid = true;
            if (ImGui::Button(ICON_MS_GRID_ON "##Grid", windowData->iconDefaultSize)) {
                showGrid = !showGrid;
                // Toggle grid visibility by changing its opacity
                windowData->nodeGraphEditor_GridOpacity = showGrid ? 1.0f : 0.0f;
                
                // Apply the grid opacity to the editor
                nodeEd::GetStyle().Colors[nodeEd::StyleColor_Grid] = ImColor(1.0f, 1.0f, 1.0f, windowData->nodeGraphEditor_GridOpacity);
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Grid");
            ImGui::SameLine();

            // Add Grid Snapping toggle
            if (ImGui::Button(ICON_MS_GRID_4X4 "##GridSnap", windowData->iconDefaultSize)) {
                // Toggle grid snapping in the state - Hydragon UI state keeping
                windowData->nodeGraphState.snapToGrid = !windowData->nodeGraphState.snapToGrid;
                
                // toggle grid snapping in imgui-node-editor code (this one is what actually toggles grid snapping.
                //nodeEd::Config::EnableGridSnap = !nodeEd::GetConfig().EnableGridSnap;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Grid Snapping");
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
        
        // Pop all the style modifications
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(4);
    }
    ImGui::EndChild();
    
    ImGui::PopStyleVar(); // Pop ChildRounding
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
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
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
