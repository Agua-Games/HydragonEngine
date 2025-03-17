/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeGraphEditor.cpp
 * @brief A specialized view for node-based graph editing.
 */
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "IconsMaterialSymbols.h"
#include <imgui.h>
#include "imgui_node_editor.h"
#include "Imgui_SteppedLineMath.h"
namespace nodeEd = ax::NodeEditor;

#include "NodeGraphEditor.h"
#include "NodeGraphState.h"
#include "UIManager.h"
//#include "Node.h"                  // For node integration - next (architecture) steps
#include "PropertyEditor.h"             // For property integration

namespace hd {

static nodeEd::EditorContext* g_NodeEditorContext = nullptr;       // imgui-node-editor context

// === For next (architecture) steps ===
// Forward declarations for sub-editor integration
//void NotifyNodeSelected(Node* node);           // Un-comment this when node integration is done
//void UpdateSubEditors(Node* selectedNode);     // And this
// === end of next steps ===

// === State Management ===
static NodeGraphState graphState;  // Instance of our state struct
struct ViewportState {             // Maybe useful in the future for canvas bookmarks etc.
    ImVec2 viewPosition = ImVec2(0.0f, 0.0f);
    float zoom = 1.0f;
} viewport;

// === Node Graph Editor - Basic Elements ===
struct PinPositionData {
    ImVec2 position;
    bool isValid;
};

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

struct LinkInfo
{
    nodeEd::LinkId Id;
    nodeEd::PinId InputId;
    nodeEd::PinId OutputId;
};

enum class LinkShape
{
    Hydragon,
    Straight,
    Bezier,
    Stepped,
};

struct LinkStyle
{
    ImColor linkColor;
    enum LinkShape linkShape;
};
LinkStyle linkStyle = {
    ImColor(1.0f, 1.0f, 1.0f, 0.2f),
    LinkShape::Bezier
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

    ImVec4 nodePadding;
    float nodeRounding;
    float pinIconSize;
};
NodeStyle nodeStyle = {
    ImColor(90, 102, 110, 255),
    ImColor(0.21f, 0.22f, 0.22f, 0.43f),
    ImColor(0.43f, 0.43f, 0.5f, 0.5f),
    ImColor(0.80f, 0.89f, 0.89f, 0.75f),
    ImVec4(0.0f, 4.0f, 0.0f, 15.0f),
    11.0f,
    9.0f
};

static bool g_FirstFrame = true;            // Flag for first frame
static ImVector<LinkInfo> g_Links;          // List of live links
static int g_NextId = 1;                    // Used to generate unique IDs

// Define a custom comparator for PinId
struct PinIdCompare {
    bool operator()(const nodeEd::PinId& a, const nodeEd::PinId& b) const {
        return a.Get() < b.Get();  // Compare the underlying IDs
    }
};

// Use the custom comparator with std::map
static std::map<nodeEd::PinId, PinPositionData, PinIdCompare> g_PinPositions;

static bool ensureNodeEditorContext() {     // Helper function to avoid silently failing by lack of context
    if (g_NodeEditorContext == nullptr) {
        return false;
    }
    // Set the current editor context if it's not already set
    nodeEd::SetCurrentEditor(g_NodeEditorContext);
    return true;
}

void initializeNodeGraphEditor(EditorWindowData* windowData) {
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

        if (ensureNodeEditorContext())
        {
            // Temporary implementation here - move to styleColorsHydragonDark(), in UIManager.cpp later.

            // Style
            nodeEd::Style& nodesStyle = nodeEd::GetStyle();
            // Customize spacing and rounding - imgui-node-editor
            nodesStyle.NodePadding = nodeStyle.nodePadding;
            nodesStyle.NodeRounding = nodeStyle.nodeRounding;
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

void shutdownNodeGraphEditor() {
    if (g_NodeEditorContext) {
        nodeEd::DestroyEditor(g_NodeEditorContext);
        g_NodeEditorContext = nullptr;
    }
}

// Improved node with title bar that aligns perfectly with node borders
void beginNodeWithTitleBar(nodeEd::NodeId nodeId, const char* title, ImColor titleBarColor, ImColor nodeColor) {
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
}

// Helper to create an input pin with custom styling
void beginInputPin(nodeEd::PinId pinId, const char* label, ImColor pinColor) {
    const float iconSize = nodeStyle.pinIconSize;
    
    nodeEd::BeginPin(pinId, nodeEd::PinKind::Input);
    nodeEd::PinPivotAlignment(ImVec2(0.0f, 0.5f));
    
    ImVec2 pinPos = ImGui::GetCursorScreenPos();
    pinPos.x += 0.0f;
    pinPos.y += ImGui::GetTextLineHeight() * 0.5f - (iconSize * 0.5f);
    
    g_PinPositions[pinId] = PinPositionData{
        ImVec2(pinPos.x + iconSize * 0.5f, pinPos.y + iconSize * 0.5f),
        true
    };

    // Draw pin icon
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 iconMin = ImVec2(pinPos.x, pinPos.y);
    ImVec2 iconMax = ImVec2(pinPos.x + iconSize, pinPos.y + iconSize);
    
    bool isConnected = nodeEd::PinHadAnyLinks(pinId);
    
    if (isConnected) {
        drawList->AddRectFilled(iconMin, iconMax, pinColor, 0.0f);
    } else {
        drawList->AddRect(iconMin, iconMax, pinColor, 0.0f, 0, 1.5f);
    }
    
    ImGui::Dummy(ImVec2(iconSize + 3.0f, ImGui::GetTextLineHeight()));
    nodeEd::EndPin();
    
    ImGui::SameLine(0, 4.0f);
    ImGui::TextUnformatted(label);
}

// Helper to create an output pin with custom styling
void beginOutputPin(nodeEd::PinId pinId, const char* label, ImColor pinColor) {
    ImGui::BeginGroup();
    const float iconSize = nodeStyle.pinIconSize;
    
    ImGui::TextUnformatted(label);
    ImGui::SameLine(0, 4.0f);
    
    nodeEd::BeginPin(pinId, nodeEd::PinKind::Output);
    nodeEd::PinPivotAlignment(ImVec2(1.0f, 0.5f));
    
    // Get and store pin position
    ImVec2 pinPos = ImGui::GetCursorScreenPos();
    pinPos.x += 0.0f;
    pinPos.y += ImGui::GetTextLineHeight() * 0.5f - (iconSize * 0.5f);
    
    // Store the position with the icon center point
    g_PinPositions[pinId] = PinPositionData{
        ImVec2(pinPos.x + 3.0f + iconSize * 0.5f, pinPos.y + iconSize * 0.5f),
        true
    };

    // Draw pin icon
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 iconMin = ImVec2(pinPos.x + 3.0f, pinPos.y);
    ImVec2 iconMax = ImVec2(pinPos.x + iconSize + 3.0f, pinPos.y + iconSize);
    
    bool isConnected = nodeEd::PinHadAnyLinks(pinId);
    
    if (isConnected) {
        drawList->AddRectFilled(iconMin, iconMax, pinColor, 0.0f);
    } else {
        drawList->AddRect(iconMin, iconMax, pinColor, 0.0f, 0, 1.5f);
    }
    
    ImGui::Dummy(ImVec2(iconSize + 3.0f, ImGui::GetTextLineHeight()));
    nodeEd::EndPin();
    ImGui::EndGroup();
}

ImVec2 getStoredPinPosition(nodeEd::PinId pinId) {
    auto it = g_PinPositions.find(pinId);
    if (it != g_PinPositions.end() && it->second.isValid) {
        return it->second.position;
    }
    return ImGui::GetMousePos(); // Fallback to mouse pos if not found
}

// You might want to add this at the beginning of your frame:
void clearPinPositions() {
    g_PinPositions.clear();
}

// Forward declare internal helper functions
static void renderNodeLibrary();
static void renderNodeLibraryContent();
static void renderGraphCanvasContent(EditorWindowData* windowData);
static void renderMiniMapContent();
static void renderTopToolbar(bool* p_open, EditorWindowData* windowData);
static void renderRightSidebar();
static void renderStatusBar();
static void renderGraphCanvas(EditorWindowData* windowData);

// Forward declarations
static bool isInputConnected(const char* inputName);

void showNodeGraphEditor(bool* p_open, EditorWindowData* windowData) 
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
            static int s_linkShape = 3;
            if (ImGui::Combo("Link Shape", &s_linkShape, "Hydragon\0Straight\0Bezier\0Stepped\0")) 
            {
                switch (s_linkShape) {
                    case 0:
                        if (ensureNodeEditorContext())
                        {
                            linkStyle.linkShape = LinkShape::Hydragon; break;
                        };
                        break;
                    case 1:
                        if (ensureNodeEditorContext())
                        {
                            linkStyle.linkShape = LinkShape::Straight;
                            nodeEd::GetStyle().LinkStrength = 0.0f;
                        };
                        break;
                    case 2:
                        if (ensureNodeEditorContext())
                        {
                            linkStyle.linkShape = LinkShape::Bezier; 
                            nodeEd::GetStyle().LinkStrength = 100.0f;
                        };
                        break;
                    case 3: linkStyle.linkShape = LinkShape::Stepped; break;
                }
            }
            if (ImGui::MenuItem("Reset View")) {
               if (ensureNodeEditorContext()) 
               {
                   nodeEd::NavigateToContent();
               }
            }
            if (ImGui::MenuItem("Frame All")) {}
            if (ImGui::MenuItem("Frame Selected")) {
                if (ensureNodeEditorContext()) 
                {
                    nodeEd::NavigateToSelection(true);
                }
            }
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
    renderTopToolbar(p_open, windowData);

    // Main Content Area - removed spacing before status bar
    ImGui::BeginChild("NodeGraphContent", ImVec2(0, -ImGui::GetFrameHeight())); 
    
    // Add resizable splitter for Node Library
    static float libraryWidth = 200.0f;
    const float minLibraryWidth = 100.0f;
    const float maxLibraryWidth = 400.0f;

    // Left panel for node library - with inner padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::BeginChild("NodeLibrary", ImVec2(libraryWidth, 0), true);
    renderNodeLibrary();
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
    renderGraphCanvas(windowData);
    ImGui::EndChild();

    ImGui::EndChild(); // End NodeGraphContent

    // Status Bar
    renderStatusBar();

    // Pop the style modifications for main layout
    ImGui::PopStyleVar(5); // Pop WindowPadding, WindowRounding, ChildRounding, ItemSpacing

    ImGui::End(); // End Node Graph
}

static void renderNodeLibrary() 
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
    renderNodeLibraryContent();

    // Make sure to pop all style vars
    ImGui::PopStyleVar(3);  // Pop all three style vars
}

static void renderNodeLibraryContent() 
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

void renderGraphCanvas(EditorWindowData* windowData)
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
    beginNodeWithTitleBar(nodeId1, "Transform", nodeStyle.titleBarColor, ImColor(60, 60, 60, 200));
    
    // Add some spacing
    ImGui::Dummy(ImVec2(0, 5));
    
    // Input pins with custom styling
    beginInputPin(inputPinId1, "Speed", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    // Output pins with custom styling
    beginOutputPin(outputPinId1, "Result1", nodeStyle.pinColor);
    
    ImGui::Dummy(ImVec2(0, 5)); // Spacing between pins
    
    beginInputPin(inputPinId2, "Orientation", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    beginOutputPin(outputPinId2, "Result2", nodeStyle.pinColor);
    
    nodeEd::EndNode();
    
    // === Begin second node with custom title bar ===
    beginNodeWithTitleBar(nodeId2, "Material", nodeStyle.titleBarColor, ImColor(60, 60, 60, 200));
    
    // Add some spacing
    ImGui::Dummy(ImVec2(0, 5));
    
    // Input pin with custom styling
    beginInputPin(inputPinId3, "Albedo", nodeStyle.pinColor);
    
    ImGui::SameLine(150); // Fixed position for output pins
    
    // Output pin with custom styling
    beginOutputPin(outputPinId3, "Mat Result", nodeStyle.pinColor);
    
    nodeEd::EndNode();
    
    // Draw existing links
    for (auto& link : g_Links)
    {
        if (linkStyle.linkShape == LinkShape::Hydragon) {
            ImVec2 startPos, endPos;
            
            if (nodeEd::QueryNewLink(nullptr, nullptr)) {  // Check if we're creating a new link
                // Get the start pin position during dragging
                nodeEd::PinId startPinId, endPinId;
                if (nodeEd::QueryNewLink(&startPinId, &endPinId)) {
                    startPos = getStoredPinPosition(startPinId);
                    endPos = ImGui::GetMousePos();
                }
            } else {
                // Normal connected link case
                if (nodeEd::GetLinkPins(link.Id, &link.InputId, &link.OutputId)) {
                    startPos = getStoredPinPosition(link.OutputId);
                    endPos = getStoredPinPosition(link.InputId);
                }
            }
            
            // Draw the Hydragon line
            SteppedLineRenderer::drawHydragonLine(
                ImGui::GetWindowDrawList(),
                startPos,
                endPos,
                linkStyle.linkColor,
                2.0f,
                false);
        }
        else if (linkStyle.linkShape == LinkShape::Stepped) {
            ImVec2 startPos, endPos;
            
            if (nodeEd::QueryNewLink(nullptr, nullptr)) {  // Check if we're creating a new link
                // Get the start pin position during dragging
                nodeEd::PinId startPinId, endPinId;
                if (nodeEd::QueryNewLink(&startPinId, &endPinId)) {
                    startPos = getStoredPinPosition(startPinId);
                    endPos = ImGui::GetMousePos();
                }
            } else {
                // Normal connected link case
                if (nodeEd::GetLinkPins(link.Id, &link.InputId, &link.OutputId)) {
                    startPos = getStoredPinPosition(link.OutputId);
                    endPos = getStoredPinPosition(link.InputId);
                }
            }
            
            // Draw the stepped line
            SteppedLineRenderer::drawSteppedLine(
                ImGui::GetWindowDrawList(),
                startPos,
                endPos,
                linkStyle.linkColor,
                2.0f,
                false);
        } 
        else 
        {
            // Original bezier/straight line drawing
            nodeEd::Link(link.Id, link.InputId, link.OutputId);
        }
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

void renderGraphCanvasContent(EditorWindowData* windowData) 
{ 

}

// Placeholder function - to be implemented properly later
static bool isInputConnected(const char* inputName) 
{
    return false;  // For now, always show widgets
}

void renderMiniMap() 
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
        renderMiniMapContent();
    }
    ImGui::End();
}

static void renderMiniMapContent() 
{
    // TODO: Render miniature version of the graph
    ImGui::Text("Mini Map");
}

static void renderTopToolbar(bool* p_open, EditorWindowData* windowData) 
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
                if (ensureNodeEditorContext()) {
                    nodeEd::NavigateToSelection(true);
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Frame Selected (F)");
            ImGui::SameLine();
            
            // Reset View button
            if (ImGui::Button(ICON_MS_RESTART_ALT "##ResetView", windowData->iconDefaultSize)) {
                // Use NavigateToContent to reset view
                if (ensureNodeEditorContext()) {
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
                if (ensureNodeEditorContext()) {
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

static void renderRightSidebar() 
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

static void renderStatusBar() 
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

} // namespace hd
