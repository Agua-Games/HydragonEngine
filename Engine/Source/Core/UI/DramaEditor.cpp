/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>

#include "IconsMaterialSymbols.h"

#include "DramaEditor.h"
#include "hdImgui.h"
#include "NodeGraphEditor.h"
#include "NodeGraphState.h"

// Forward declarations
namespace hdImgui {
    struct HdEditorWindowData;
}

// Define the state structure first
struct DramaEditorState {
    bool showBrowser = true;
    bool showProperties = true;
    bool showTimeline = true;
    bool showDebug = false;
    
    // Current selection/focus
    int selectedDramaSequence = -1;
    int selectedNode = -1;
    float currentTime = 0.0f;
    
    // View states
    bool isPlaying = false;
    bool showTensionCurve = true;
    bool showStateSpace = false;
    bool showPreview = false;
    
    // Scripting related states
    bool showScriptPanel = false;
    int activeScriptTab = 0;  // 0: Visual, 1: Code
    std::string selectedScriptLanguage = "Blueprint";  // Default to visual scripting
    
    // State System UI states
    bool showStateSpacePanel = false;
    bool showStateDebugger = false;
    float stateSpaceZoom = 1.0f;
    ImVec2 stateSpaceOffset = ImVec2(0, 0);
    
    // Timeline integration states
    bool showTimelinePanel = true;
    float dramaTime = 0.0f;        // Current time in drama sequence
    bool isTimelineSynced = true;  // Sync with MontageEditor
    int activeMontageId = -1;      // Currently linked montage
    
    // Tension System states
    bool showTensionPanel = false;
    float currentTension = 0.0f;
    bool showTensionGraph = true;
    bool autoAdjustTension = false;
};

// Static state instance
static DramaEditorState* state = nullptr;

// Forward declare ScriptingInterface
struct ScriptingInterface;

struct DramaNodeScriptingIntegration {
    ScriptingInterface* scriptInterface;
    
    void SetupNodeScript(int nodeId) {
        // TODO: Implement node script setup
    }
};

// Placeholder functions
static void UpdateNodeStates() {
    // TODO: Implement node state updates during playback
}

static void UpdateNodeVisuals() {
    // TODO: Implement node visual updates
}

static void UpdateTensionCurve() {
    // TODO: Implement tension curve visualization
}

static void UpdateStateSpaceView() {
    // TODO: Implement state space visualization
}

// Node creation functions
static void CreateEventNode() {
    // TODO: Implement event node creation
}

static void CreateStateNode() {
    // TODO: Implement state node creation
}

static void CreateTransitionNode() {
    // TODO: Implement transition node creation
}

static void CreateActionNode() {
    // TODO: Implement action node creation
}

// Connection validation
static bool ValidateConnection(int fromId, int toId) {
    return true; // TODO: Implement proper validation
}

struct DramaEditorNodeIntegration {
    hdImgui::NodeGraphState* graphState = nullptr;
    bool initialized = false;
    
    DramaEditorNodeIntegration() {
        if (!state) {
            state = new DramaEditorState();
        }
    }
    
    ~DramaEditorNodeIntegration() {
        if (state) {
            delete state;
            state = nullptr;
        }
    }
    
    void Initialize() {
        if (initialized) return;
        
        graphState = new hdImgui::NodeGraphState();
        RegisterDramaNodes();
        SetupNodeCallbacks();
        
        initialized = true;
    }
    
    void Shutdown() {
        if (graphState) {
            delete graphState;
            graphState = nullptr;
        }
        initialized = false;
    }
    
    void PreNodeGraphRender() {
        if (!initialized || !graphState) return;
        // Prepare the node graph for rendering
        // Update node states, positions, etc.
    }
    
    void PostNodeGraphRender() {
        if (!initialized || !graphState) return;
        // Clean up after rendering
        // Update any post-render states
    }
    
    void RegisterDramaNodes() {
        if (!graphState) return;
        graphState->RegisterNodeType("Event", CreateEventNode);
        graphState->RegisterNodeType("State", CreateStateNode);
        graphState->RegisterNodeType("Transition", CreateTransitionNode);
        graphState->RegisterNodeType("Action", CreateActionNode);
    }
    
    void SetupNodeCallbacks() {
        if (!graphState) return;
        graphState->onNodeConnect = ValidateConnection;
        
        graphState->onNodeSelected = [](int nodeId) {
            if (state) {
                state->selectedNode = nodeId;
            }
        };
    }
};

// Static instance of node integration
static DramaEditorNodeIntegration nodeIntegration;

// Forward declarations of the panel rendering functions
static void ShowDramaBrowser()
{
    ImGui::Text("Drama Browser");
    // Placeholder until Phase 2
}

static void ShowMainWorkspace(hdImgui::HdEditorWindowData* windowData)
{
    ImGui::BeginChild("MainWorkspace", ImVec2(0, 0), true);
    
    nodeIntegration.PreNodeGraphRender();  // Now nodeIntegration is defined
    
    bool isOpen = true;
    hdImgui::ShowNodeGraphEditor(&isOpen, windowData);
    
    nodeIntegration.PostNodeGraphRender();  // Now nodeIntegration is defined
    
    ImGui::EndChild();
}

static void ShowPropertiesPanel()
{
    ImGui::Text("Properties Panel");
    // Placeholder until Phase 2
}

static void ShowTimeline()
{
    ImGui::Text("Timeline");
    // Placeholder until Phase 2
}

static void ShowScriptPanel()
{
    if (!state || !state->showScriptPanel) return;
    
    ImGui::Begin("Node Script", &state->showScriptPanel);
    
    // Language selector
    const char* languages[] = { "Blueprint", "C++", "Python", "Lua" };
    if (ImGui::BeginCombo("Language", state->selectedScriptLanguage.c_str()))
    {
        for (const char* lang : languages)
        {
            if (ImGui::Selectable(lang, state->selectedScriptLanguage == lang))
            {
                state->selectedScriptLanguage = lang;
            }
        }
        ImGui::EndCombo();
    }

    // Script/Visual tabs
    ImGui::BeginTabBar("ScriptingTabs");
    if (ImGui::BeginTabItem("Visual"))
    {
        ImGui::Text("Visual Script Editor");
        // Placeholder for visual scripting interface
        // Will interface with NodeGraphEditor later
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Code"))
    {
        ImGui::Text("Code Editor");
        // Placeholder for code editor interface
        // Will interface with ScriptEditor later
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();

    ImGui::End();
}

static void ShowStateSpacePanel()
{
    if (!state || !state->showStateSpacePanel) return;
    
    ImGui::Begin("State Space", &state->showStateSpacePanel);
    
    // Toolbar
    if (ImGui::BeginChild("StateSpaceToolbar", ImVec2(0, 30), true))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
        if (ImGui::Button("Reset View")) {
            state->stateSpaceZoom = 1.0f;
            state->stateSpaceOffset = ImVec2(0, 0);
        }
        ImGui::SameLine();
        ImGui::Button("Fit View");
        ImGui::SameLine();
        ImGui::Button("Center Selected");
        ImGui::SameLine();
        ImGui::Checkbox("Show Labels", &state->showDebug);
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    // Main view
    if (ImGui::BeginChild("StateSpaceView", ImVec2(0, 0), true))
    {
        ImGui::Text("State Space View - Will be populated by node system");
        // Placeholder for actual state space visualization
        // Will be connected to NodeGraphEditor's node system
    }
    ImGui::EndChild();
    
    ImGui::End();
}

static void ShowStateDebugger()
{
    if (!state || !state->showStateDebugger) return;
    
    ImGui::Begin("State Debugger", &state->showStateDebugger);
    
    if (ImGui::BeginTabBar("StateDebuggerTabs"))
    {
        if (ImGui::BeginTabItem("Current State"))
        {
            ImGui::Text("Active States:");
            ImGui::Separator();
            // Placeholder for active states list
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("History"))
        {
            ImGui::Text("State Transitions:");
            ImGui::Separator();
            // Placeholder for state transition history
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Variables"))
        {
            ImGui::Text("State Variables:");
            ImGui::Separator();
            // Placeholder for state variables watch
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

static void ShowTimelinePanel(hdImgui::HdEditorWindowData* windowData)
{
    if (!state || !state->showTimelinePanel) return;
    
    ImGui::Begin("Drama Timeline", &state->showTimelinePanel);
    
    // Toolbar with montage linking controls
    if (ImGui::BeginChild("TimelineToolbar", ImVec2(0, 30), true))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
        
        // Montage linking
        if (ImGui::Button(ICON_MS_LINK "##LinkMontage"))
        {
            // TODO: Hook - Open montage selector
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Link to Montage");
            
        ImGui::SameLine();
        ImGui::Text("Active Montage: %d", state->activeMontageId);
        
        ImGui::SameLine();
        ImGui::Checkbox("Sync", &state->isTimelineSynced);
        
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    // Timeline overview
    if (ImGui::BeginChild("TimelineOverview", ImVec2(0, 100), true))
    {
        // Display current drama time and markers
        ImGui::Text("Drama Time: %.2f", state->dramaTime);
        
        // Placeholder for drama event markers
        ImGui::Text("Drama Events Timeline - Will sync with MontageEditor");
    }
    ImGui::EndChild();
    
    ImGui::End();
}

static void ShowTensionPanel()
{
    if (!state || !state->showTensionPanel) return;
    
    ImGui::Begin("Drama Tension", &state->showTensionPanel);
    
    // Tension controls toolbar
    if (ImGui::BeginChild("TensionToolbar", ImVec2(0, 30), true))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
        
        ImGui::Checkbox("Auto-Adjust", &state->autoAdjustTension);
        ImGui::SameLine();
        
        ImGui::Button(ICON_MS_RESET_TV "##ResetTension");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Reset Tension");
            
        ImGui::SameLine();
        ImGui::Checkbox("Show Graph", &state->showTensionGraph);
        
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();

    // Current tension slider
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::VSliderFloat("##TensionLevel", ImVec2(30, 150), &state->currentTension, 0.0f, 1.0f, "");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Tension: %.2f", state->currentTension);
    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();

    // Tension graph and events
    if (state->showTensionGraph)
    {
        if (ImGui::BeginChild("TensionGraph", ImVec2(0, 150), true))
        {
            ImGui::Text("Tension Graph - Will show tension curve over time");
            // Placeholder for tension curve visualization
            // Will be connected to drama events and state system
        }
        ImGui::EndChild();
    }

    // Tension events list
    if (ImGui::BeginChild("TensionEvents", ImVec2(0, 0), true))
    {
        ImGui::Text("Tension Events");
        ImGui::Separator();
        
        if (ImGui::BeginTable("TensionEventsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            ImGui::TableSetupColumn("Time");
            ImGui::TableSetupColumn("Event");
            ImGui::TableSetupColumn("Impact");
            ImGui::TableHeadersRow();
            
            // Placeholder for tension events list
            // Will be populated from drama events system
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("0:00");
            ImGui::TableNextColumn();
            ImGui::Text("Start");
            ImGui::TableNextColumn();
            ImGui::Text("+0.2");
            
            ImGui::EndTable();
        }
    }
    ImGui::EndChild();
    
    ImGui::End();
}

struct DramaMontageSync {
    static void OnMontageTimeChanged(float newTime) {
        if (state && state->isTimelineSynced) {
            state->dramaTime = newTime;
            // TODO: Hook - Update drama state based on time
        }
    }
    
    static void OnDramaTimeChanged(float newTime) {
        if (state && state->isTimelineSynced) {
            // TODO: Hook - Call MontageEditor's time update
            // windowData->montageEditor->SetTime(newTime);
        }
    }
    
    static void OnMontageLinked(int montageId) {
        if (state) {
            state->activeMontageId = montageId;
            // TODO: Hook - Setup drama-montage relationship
        }
    }
};

namespace hdImgui {
void ShowDramaEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    // Initialize node integration if needed
    nodeIntegration.Initialize();
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
    
    if (!ImGui::Begin("Drama Editor", p_open, window_flags))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Drama Sequence", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("Drama Template...")) {}
                if (ImGui::MenuItem("State Space...")) {}
                if (ImGui::MenuItem("Tensor Config...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("As Template...")) {}
                if (ImGui::MenuItem("State Configuration...")) {}
                if (ImGui::MenuItem("Analytics Data...")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate Sequence", "Ctrl+D")) {}
            if (ImGui::MenuItem("Delete", "Del")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Drama Browser", nullptr, &state->showBrowser);
            ImGui::MenuItem("Properties Panel", nullptr, &state->showProperties);
            ImGui::MenuItem("Timeline Overview##MainTimeline", nullptr, &state->showTimeline);
            ImGui::MenuItem("Debug Overlay", nullptr, &state->showDebug);
            ImGui::MenuItem("Script Panel", nullptr, &state->showScriptPanel);
            ImGui::MenuItem("State Space", nullptr, &state->showStateSpacePanel);
            ImGui::MenuItem("State Debugger", nullptr, &state->showStateDebugger);
            ImGui::MenuItem("Drama Timeline##DockPanel", nullptr, &state->showTimelinePanel);
            ImGui::MenuItem("Tension System##TensionPanel", nullptr, &state->showTensionPanel);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Main Layout
    const float browserWidth = 250.0f;
    const float propertiesWidth = 300.0f;
    const float timelineHeight = 150.0f;

    // Left Panel - Drama Browser
    if (state->showBrowser)
    {
        ImGui::BeginChild("DramaBrowser", ImVec2(browserWidth, -timelineHeight), true);
        ShowDramaBrowser();
        ImGui::EndChild();
    }

    ImGui::SameLine();

    // Center Panel - Main Workspace
    ImGui::BeginChild("MainWorkspace", ImVec2(-propertiesWidth - 8.0f, -timelineHeight), true);
    ShowMainWorkspace(windowData);
    ImGui::EndChild();

    ImGui::SameLine();

    // Right Panel - Properties
    if (state->showProperties)
    {
        ImGui::BeginChild("Properties", ImVec2(0, -timelineHeight), true);
        ShowPropertiesPanel();
        ImGui::EndChild();
    }

    // Bottom Panel - Timeline
    if (state->showTimeline)
    {
        ImGui::BeginChild("Timeline", ImVec2(0, 0), true);
        ShowTimeline();
        ImGui::EndChild();
    }

    if (state->showScriptPanel)
    {
        ShowScriptPanel();
    }

    if (state->showStateSpacePanel)
    {
        ShowStateSpacePanel();
    }
    if (state->showStateDebugger)
    {
        ShowStateDebugger();
    }

    if (state->showTimelinePanel)
    {
        ShowTimelinePanel(windowData);
    }

    if (state->showTensionPanel)
    {
        ShowTensionPanel();
    }

    ImGui::End();

    if (!*p_open) {
        nodeIntegration.Shutdown();
    }
}
} // namespace hdImgui
