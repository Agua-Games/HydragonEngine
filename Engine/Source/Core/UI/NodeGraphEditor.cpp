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

static nodeEd::EditorContext* g_NodeEditorContext = nullptr;       // imgui-node-editor context

// === For next (architecture) steps ===
// Forward declarations for sub-editor integration
//void NotifyNodeSelected(HD_Node* node);           // Un-comment this when node integration is done
//void UpdateSubEditors(HD_Node* selectedNode);     // And this
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

struct NodeStyle{
    ImColor titleBarColor;
    ImColor nodeBgColor;
    ImColor nodeBorderColor;
    ImColor pinColor;
    float pinIconSize;
    ImVec4 nodePadding;
};
NodeStyle nodeStyle = {
    ImColor(90, 102, 110, 255),
    ImColor(0.21f, 0.22f, 0.22f, 0.43f),
    ImColor(0.43f, 0.43f, 0.5f, 0.5f),
    ImColor(0.80f, 0.89f, 0.89f, 0.75f),
    9.0f,
    ImVec4(0.0f, 4.0f, 0.0f, 15.0f)
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

static bool EnsureNodeEditorContext() {     // Helper function to avoid silently failing by lack of context
    if (g_NodeEditorContext == nullptr) {
        return false;
    }
    
    // Set the current editor context if it's not already set
    nodeEd::SetCurrentEditor(g_NodeEditorContext);
    return true;
}

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

        if (EnsureNodeEditorContext())
        {
            // Temporary implementation here - move to StyleColorsHydragonDark(), in hdImgui.cpp later.

            // Style
            nodeEd::Style& nodesStyle = nodeEd::GetStyle();
            // Customize spacing and rounding - imgui-node-editor
            nodesStyle.NodePadding = nodeStyle.nodePadding;
            nodesStyle.NodeRounding = 11.0f;
            nodesStyle.NodeBorderWidth = 1.6f;
            nodesStyle.PinRounding = 0.0f;
            //nodesStyle.SnapLinkToPinDir = 1.0f;
            nodesStyle.LinkStrength = 100.0f;           // move strength to menu entry to toggle between bezier and straight lines

            // Style colors
            nodesStyle.Colors[nodeEd::StyleColor_Bg] = ImColor(0.21f, 0.22f, 0.22f, 1.0f);
            nodesStyle.Colors[nodeEd::StyleColor_Grid] = ImColor(0.27f, 0.28f, 0.28f, 0.5f);
            nodesStyle.Colors[nodeEd::StyleColor_NodeBg] = nodeStyle.nodeBgColor;
            nodesStyle.Colors[nodeEd::StyleColor_NodeBorder] = ImColor(0.43f, 0.43f, 0.5f, 0.5f);
            nodesStyle.Colors[nodeEd::StyleColor_SelLinkBorder] = ImColor(0.43f, 0.43f, 0.5f, 0.5f);
        }
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

// Improved node with title bar that aligns perfectly with node borders
void BeginNodeWithTitleBar(nodeEd::NodeId nodeId, const char* title, ImColor titleBarColor, ImColor nodeColor) {
    // Begin the node
    nodeEd::BeginNode(nodeId);
    
    // Calculate node width based on content
    float nodeWidth = ImGui::CalcTextSize(title).x + 40.0f;
    nodeWidth = std::max(nodeWidth, 120.0f); // Minimum width
    
    // Get the node editor style to match rounding
    float cornerRounding = nodeEd::GetStyle().NodeRounding;
    
    // Create a dummy for the title bar
    ImGui::Dummy(ImVec2(nodeWidth, 24.0f));
    
    // Get the rect for the title bar
    ImVec2 rectMin = ImGui::GetItemRectMin();
    ImVec2 rectMax = ImGui::GetItemRectMax();
    
    // Get node position/size to align header perfectly
    ImVec2 nodePos = nodeEd::GetNodePosition(nodeId);
    ImVec2 nodeSize = nodeEd::GetNodeSize(nodeId);
    
    // Draw title bar using node's full width with zero padding
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(
        nodePos,
        ImVec2(nodePos.x + nodeSize.x, nodePos.y + 24.0f), // Fixed header height
        titleBarColor,
        cornerRounding,
        ImDrawFlags_RoundCornersTop
    );

    // Draw title text
    ImVec2 textSize = ImGui::CalcTextSize(title);
    drawList->AddText(
        ImVec2(
            nodePos.x + (nodeSize.x - textSize.x) * 0.5f,
            nodePos.y + (24.0f - textSize.y) * 0.5f
        ),
        // Match text color with Hydragon Style - *later move this to proper solid, organized place.
        IM_COL32(225, 225, 225, 255),
        title
    );
    
    // Small spacing after title bar
    ImGui::Dummy(ImVec2(0, 4.0f));
    
    // Begin a group to constrain the node width
    ImGui::BeginGroup();
}

// End the node with title bar
void EndNodeWithTitleBar() {
    // End the group that constrains node width
    ImGui::EndGroup();
    nodeEd::EndNode();
}

// Helper to create an input pin with custom styling
void BeginInputPin(nodeEd::PinId pinId, const char* label, ImColor pinColor) {
    const float iconSize = nodeStyle.pinIconSize;
    
    // Begin the pin
    nodeEd::BeginPin(pinId, nodeEd::PinKind::Input);
    
    // Set pin pivot alignment to left
    nodeEd::PinPivotAlignment(ImVec2(0.0f, 0.5f));
    
    // Get pin position for visual icon
    ImVec2 pinPos = ImGui::GetCursorScreenPos();
    // Position at left border, accounting for icon size
    pinPos.x += 0.0f; // Add slight offset to touch inner border, if needed
    pinPos.y += ImGui::GetTextLineHeight() * 0.5f - (iconSize * 0.5f); // Center vertically

    // Draw pin icon
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 iconMin = ImVec2(pinPos.x, pinPos.y);
    ImVec2 iconMax = ImVec2(pinPos.x + iconSize, pinPos.y + iconSize);
    
    // Check if pin is connected
    bool isConnected = nodeEd::PinHadAnyLinks(pinId);
    
    if (isConnected) {
        drawList->AddRectFilled(iconMin, iconMax, pinColor, 0.0f);
    } else {
        drawList->AddRect(iconMin, iconMax, pinColor, 0.0f, 0, 1.5f);
    }
    
    // Add spacing for the pin icon
    ImGui::Dummy(ImVec2(iconSize + 3.0f, ImGui::GetTextLineHeight()));
    
    // End the pin
    nodeEd::EndPin();
    
    // Display the label after the pin
    ImGui::SameLine(0, 4.0f);
    ImGui::TextUnformatted(label);
}

// Helper to create an output pin with custom styling
void BeginOutputPin(nodeEd::PinId pinId, const char* label, ImColor pinColor) {
    // Begin a group for this pin to ensure proper layout
    ImGui::BeginGroup();

    const float iconSize = nodeStyle.pinIconSize;
    
    // Display the label first
    ImGui::TextUnformatted(label);
    ImGui::SameLine(0, 4.0f);
    
    // Begin the pin
    nodeEd::BeginPin(pinId, nodeEd::PinKind::Output);
    
    // Set pin pivot alignment to right
    nodeEd::PinPivotAlignment(ImVec2(1.0f, 0.5f));
    
    // Calculate positions for visual and interaction areas
    
    
    // Get pin position for visual icon.
    ImVec2 pinPos = ImGui::GetCursorScreenPos();
    // Position at right border, accounting for icon size
    pinPos.x += 0.0f;
    pinPos.y += ImGui::GetTextLineHeight() * 0.5f - (iconSize * 0.5f);     // Center vertically

    // Draw pin icon
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 iconMin = ImVec2(pinPos.x, pinPos.y);
    ImVec2 iconMax = ImVec2(pinPos.x + iconSize, pinPos.y + iconSize);
    
    // Check if pin is connected
    bool isConnected = nodeEd::PinHadAnyLinks(pinId);
    
    if (isConnected) {
        drawList->AddRectFilled(iconMin, iconMax, pinColor, 0.0f);
    } else {
        drawList->AddRect(iconMin, iconMax, pinColor, 0.0f, 0, 1.5f);
    }
    
    // Add dummy for proper pin interaction area
    ImGui::Dummy(ImVec2(iconSize + 3.0f, ImGui::GetTextLineHeight()));
    
    // End the pin
    nodeEd::EndPin();
    
    ImGui::EndGroup();
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
    static nodeEd::PinId inputPinId2 = 3;
    static nodeEd::PinId outputPinId1 = 4;
    static nodeEd::PinId outputPinId2 = 5;
    
    // === SECOND NODE ===
    static nodeEd::NodeId nodeId2 = 6;
    static nodeEd::PinId inputPinId3 = 7;
    static nodeEd::PinId outputPinId3 = 8;
    
    // Set node positions only once
    if (g_FirstFrame)
    {
        nodeEd::SetNodePosition(nodeId1, ImVec2(200, 200));
        nodeEd::SetNodePosition(nodeId2, ImVec2(500, 200));
        g_FirstFrame = false;
    }
    
    // === Begin first node with custom title bar ===
    BeginNodeWithTitleBar(nodeId1, "Transform", nodeStyle.titleBarColor, ImColor(60, 60, 60, 200));
    
    // Add some spacing
    ImGui::Dummy(ImVec2(0, 5));
    
    // Input pins with custom styling
    BeginInputPin(inputPinId1, "Speed", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    // Output pins with custom styling
    BeginOutputPin(outputPinId1, "Result1", nodeStyle.pinColor);
    
    ImGui::Dummy(ImVec2(0, 5)); // Spacing between pins
    
    BeginInputPin(inputPinId2, "Orientation", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    BeginOutputPin(outputPinId2, "Result2", nodeStyle.pinColor);
    
    nodeEd::EndNode();
    
    // === Begin second node with custom title bar ===
    BeginNodeWithTitleBar(nodeId2, "Material", nodeStyle.titleBarColor, ImColor(60, 60, 60, 200));
    
    // Add some spacing
    ImGui::Dummy(ImVec2(0, 5));
    
    // Input pin with custom styling
    BeginInputPin(inputPinId3, "Albedo", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    // Output pin with custom styling
    BeginOutputPin(outputPinId3, "Mat Result", nodeStyle.pinColor);
    
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
                if (EnsureNodeEditorContext()) {
                    nodeEd::NavigateToSelection(true);
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Frame Selected (F)");
            ImGui::SameLine();
            
            // Reset View button
            if (ImGui::Button(ICON_MS_RESTART_ALT "##ResetView", windowData->iconDefaultSize)) {
                // Use NavigateToContent to reset view
                if (EnsureNodeEditorContext()) {
                    nodeEd::NavigateToContent();
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reset View");
            ImGui::SameLine();
            
            // Add Grid toggle using opacity
            static bool showGrid = true;
            if (ImGui::Button(ICON_MS_GRID_ON "##Grid", windowData->iconDefaultSize)) {
                showGrid = !showGrid;
                // Toggle grid visibility by changing its opacity
                windowData->nodeGraphEditor_GridOpacity = showGrid ? 0.5f : 0.0f;
                
                // Apply the grid opacity to the editor
                if (EnsureNodeEditorContext()) {
                    nodeEd::GetStyle().Colors[nodeEd::StyleColor_Grid] = ImColor(0.27f, 0.28f, 0.28f, windowData->nodeGraphEditor_GridOpacity);   // Temporary hardcoded values. Fix later!
                }
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
    ImGui::BeginGroup(); // Start output pin group
    ImGui::Text("FPS: %.1f", graphState.fps);
    ImGui::SameLine();
    ImGui::Text("| Mem: %.1f MB", graphState.graphMemoryUsage);
    ImGui::EndGroup(); // End output pin group
    
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
