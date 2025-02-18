/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>

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
            ImGui::MenuItem("Timeline", nullptr, &state->showTimeline);
            ImGui::MenuItem("Debug Overlay", nullptr, &state->showDebug);
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

    ImGui::End();

    if (!*p_open) {
        nodeIntegration.Shutdown();
    }
}
} // namespace hdImgui
