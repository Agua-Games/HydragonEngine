/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>

#include "IconsMaterialSymbols.h"

#include "PhysicsEditor.h"
#include "hdImgui.h"


namespace hdImgui {

// Forward declarations of internal functions
static void ShowPhysicsToolbar(HdEditorWindowData* windowData);
static void ShowSolverPanel(HdEditorWindowData* windowData);
static void ShowRigPanel(HdEditorWindowData* windowData);
static void ShowConstraintsPanel(HdEditorWindowData* windowData);
static void ShowProceduralPanel(HdEditorWindowData* windowData);
static void ShowSimulationPanel(HdEditorWindowData* windowData);
static void ShowOptimizationPanel(HdEditorWindowData* windowData);

static void ShowSolverPanel(HdEditorWindowData* windowData)
{
    // Left panel - Solver hierarchy and types
    ImGui::BeginChild("SolverLeft", ImVec2(200, 0), true);
    {
        if (ImGui::TreeNode("Main Solvers"))
        {
            ImGui::Selectable("Standard Physics Solver");
            ImGui::Selectable("GPU-Accelerated Solver");
            ImGui::Selectable("Multi-threaded Solver");
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("Mini-Solvers"))
        {
            ImGui::Selectable("Character Motion Solver");
            ImGui::Selectable("Cloth Mini-Solver");
            ImGui::Selectable("Particle Mini-Solver");
            ImGui::Selectable("Fluid Mini-Solver");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Custom Solvers"))
        {
            ImGui::Selectable("Add Custom Solver...");
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel - Solver properties
    ImGui::BeginChild("SolverRight", ImVec2(0, 0), true);
    {
        static int executionMode = 0;
        static int iterationCount = 10;
        static float timeStep = 0.016f;
        static float subSteps = 1.0f;
        static bool enableLOD = false;
        static bool useSpatialPartitioning = true;
        static bool enableSleep = true;
        static float sleepThreshold = 0.1f;
        static bool profileSolver = false;
        static bool showStats = true;
        static bool validateSolutions = false;
        static bool exportSolverData = false;
        static char usdPath[256] = "";

        if (ImGui::CollapsingHeader("Solver Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Combo("Execution Mode", &executionMode, "CPU Single-Thread\0CPU Multi-Thread\0GPU Compute\0Hybrid\0");
            ImGui::DragInt("Iteration Count", &iterationCount, 1, 1, 100);
            ImGui::DragFloat("Time Step", &timeStep, 0.001f, 0.001f, 0.1f);
            ImGui::DragFloat("Sub Steps", &subSteps, 1, 1, 10);
        }

        if (ImGui::CollapsingHeader("Optimization", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable LOD", &enableLOD);
            ImGui::Checkbox("Use Spatial Partitioning", &useSpatialPartitioning);
            ImGui::Checkbox("Enable Sleep", &enableSleep);
            ImGui::DragFloat("Sleep Threshold", &sleepThreshold, 0.01f, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Profile Solver", &profileSolver);
            ImGui::Checkbox("Show Statistics", &showStats);
            ImGui::Checkbox("Validate Solutions", &validateSolutions);
        }

        if (ImGui::CollapsingHeader("USD Integration", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Export Solver Data", &exportSolverData);
            ImGui::InputText("USD Path", usdPath, sizeof(usdPath));
            if (ImGui::Button("Export to USD")) {}
        }
    }
    ImGui::EndChild();
}

void ShowPhysicsEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Physics", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Physics Setup", "Ctrl+N")) {}
            if (ImGui::MenuItem("Import Physics...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Import Preset"))
            {
                if (ImGui::MenuItem("Character Controller")) {}
                if (ImGui::MenuItem("Vehicle")) {}
                if (ImGui::MenuItem("Cloth")) {}
                if (ImGui::MenuItem("Fluid")) {}
                if (ImGui::MenuItem("Soft Body")) {}
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Export to USD")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Copy Settings", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste Settings", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Show Physics Debug", nullptr, &windowData->tempBool);
            ImGui::MenuItem("Show Collision Shapes", nullptr, &windowData->tempBool);
            ImGui::MenuItem("Show Constraints", nullptr, &windowData->tempBool);
            ImGui::MenuItem("Show Forces", nullptr, &windowData->tempBool);
            ImGui::MenuItem("Show Velocity", nullptr, &windowData->tempBool);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar
    ShowPhysicsToolbar(windowData);

    // Main Content Area
    ImGui::BeginChild("PhysicsContent", ImVec2(0, 0), false);
    {
        if (ImGui::BeginTabBar("PhysicsTabs"))
        {
            // Solver Configuration Tab
            if (ImGui::BeginTabItem("Solvers"))
            {
                ShowSolverPanel(windowData);
                ImGui::EndTabItem();
            }

            // Physics Rig Tab
            if (ImGui::BeginTabItem("Rigs"))
            {
                ShowRigPanel(windowData);
                ImGui::EndTabItem();
            }

            // Constraints Tab
            if (ImGui::BeginTabItem("Constraints"))
            {
                ShowConstraintsPanel(windowData);
                ImGui::EndTabItem();
            }

            // Procedural Tab
            if (ImGui::BeginTabItem("Procedural"))
            {
                ShowProceduralPanel(windowData);
                ImGui::EndTabItem();
            }

            // Simulation Tab
            if (ImGui::BeginTabItem("Simulation"))
            {
                ShowSimulationPanel(windowData);
                ImGui::EndTabItem();
            }

            // Optimization Tab
            if (ImGui::BeginTabItem("Optimization"))
            {
                ShowOptimizationPanel(windowData);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

// Implement toolbar with consistent styling
static void ShowPhysicsToolbar(HdEditorWindowData* windowData)
{
    const float toolbarHeight = 30.0f;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    
    if (ImGui::BeginChild("PhysicsToolbar", ImVec2(-1, toolbarHeight), true, 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

        // Simulation Control
        if (ImGui::Button(ICON_MS_PLAY_ARROW "##Play", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play Simulation");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_PAUSE "##Pause", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Pause Simulation");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_STOP "##Stop", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop Simulation");
        ImGui::SameLine();

        ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

        // Rig Tools
        if (ImGui::Button(ICON_MS_ADD "##AddRig", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Physics Rig");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_CONTENT_COPY "##DuplicateRig", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Duplicate Rig");
        ImGui::SameLine();

        ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

        // Debug Views
        if (ImGui::Button(ICON_MS_BUG_REPORT "##DebugView", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Debug View");
        ImGui::SameLine();

        if (ImGui::Button(ICON_MS_SETTINGS "##Settings", windowData->iconDefaultSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Physics Settings");

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(4);
    }
    ImGui::EndChild();
}

static void ShowRigPanel(HdEditorWindowData* windowData)
{
    // Left panel - Rig hierarchy
    ImGui::BeginChild("RigLeft", ImVec2(200, 0), true);
    {
        if (ImGui::TreeNode("Active Rigs"))
        {
            ImGui::Selectable("Character Rig");
            ImGui::Selectable("Vehicle Rig");
            ImGui::Selectable("Cloth Rig");
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNode("Rig Templates"))
        {
            ImGui::Selectable("Humanoid");
            ImGui::Selectable("Quadruped");
            ImGui::Selectable("Vehicle");
            ImGui::Selectable("Cloth");
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel - Rig properties
    ImGui::BeginChild("RigRight", ImVec2(0, 0), true);
    {
        static bool enableRig = true;
        static float mass = 1.0f;
        static float friction = 0.5f;
        static float restitution = 0.5f;
        static int rigType = 0;
        static bool useGravity = true;
        static bool isKinematic = false;
        static float linearDamping = 0.01f;
        static float angularDamping = 0.01f;

        if (ImGui::CollapsingHeader("Rig Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable Rig", &enableRig);
            ImGui::Combo("Rig Type", &rigType, "Dynamic\0Kinematic\0Static\0");
            ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 1000.0f);
            ImGui::DragFloat("Friction", &friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Physics Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Use Gravity", &useGravity);
            ImGui::Checkbox("Is Kinematic", &isKinematic);
            ImGui::DragFloat("Linear Damping", &linearDamping, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Angular Damping", &angularDamping, 0.01f, 0.0f, 1.0f);
        }
    }
    ImGui::EndChild();
}

static void ShowConstraintsPanel(HdEditorWindowData* windowData)
{
    static int selectedConstraint = 0;
    static bool enableConstraint = true;
    static float breakForce = 100.0f;
    static float breakTorque = 100.0f;

    // Left panel - Constraints list
    ImGui::BeginChild("ConstraintsLeft", ImVec2(200, 0), true);
    {
        if (ImGui::TreeNode("Joint Constraints"))
        {
            ImGui::Selectable("Hinge Joint");
            ImGui::Selectable("Ball Joint");
            ImGui::Selectable("Fixed Joint");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Motion Constraints"))
        {
            ImGui::Selectable("Distance");
            ImGui::Selectable("Spring");
            ImGui::Selectable("Slider");
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel - Constraint properties
    ImGui::BeginChild("ConstraintsRight", ImVec2(0, 0), true);
    {
        if (ImGui::CollapsingHeader("Constraint Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable Constraint", &enableConstraint);
            ImGui::DragFloat("Break Force", &breakForce, 1.0f, 0.0f, 10000.0f);
            ImGui::DragFloat("Break Torque", &breakTorque, 1.0f, 0.0f, 10000.0f);
        }

        if (ImGui::CollapsingHeader("Limits", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static float linearLimit = 1.0f;
            static float angularLimit = 45.0f;
            ImGui::DragFloat("Linear Limit", &linearLimit, 0.1f, 0.0f, 100.0f);
            ImGui::DragFloat("Angular Limit", &angularLimit, 1.0f, 0.0f, 360.0f);
        }
    }
    ImGui::EndChild();
}

static void ShowProceduralPanel(HdEditorWindowData* windowData)
{
    static int selectedGenerator = 0;
    static bool enableProcedural = true;
    
    // Left panel - Generators list
    ImGui::BeginChild("ProceduralLeft", ImVec2(200, 0), true);
    {
        if (ImGui::TreeNode("Physics Generators"))
        {
            ImGui::Selectable("Terrain Generator");
            ImGui::Selectable("Particle System");
            ImGui::Selectable("Cloth Generator");
            ImGui::Selectable("Fluid Generator");
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Custom Generators"))
        {
            ImGui::Selectable("Add New Generator...");
            ImGui::TreePop();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Right panel - Generator properties
    ImGui::BeginChild("ProceduralRight", ImVec2(0, 0), true);
    {
        if (ImGui::CollapsingHeader("Generator Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable Generator", &enableProcedural);
            static int seed = 12345;
            static float density = 1.0f;
            static float complexity = 0.5f;
            
            ImGui::InputInt("Seed", &seed);
            ImGui::DragFloat("Density", &density, 0.01f, 0.0f, 10.0f);
            ImGui::DragFloat("Complexity", &complexity, 0.01f, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Generation Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static bool autoGenerate = false;
            static float updateInterval = 1.0f;
            
            ImGui::Checkbox("Auto Generate", &autoGenerate);
            if (autoGenerate)
            {
                ImGui::DragFloat("Update Interval", &updateInterval, 0.1f, 0.1f, 10.0f);
            }
            
            if (ImGui::Button("Generate Now")) {}
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {}
        }
    }
    ImGui::EndChild();
}

static void ShowSimulationPanel(HdEditorWindowData* windowData)
{
    static bool simulationRunning = false;
    static float simulationSpeed = 1.0f;
    static int substeps = 2;
    static float fixedTimestep = 0.016f;
    static bool enableContinuousCollision = true;
    static bool enableAdaptiveTimestep = false;
    static float maxTimestep = 0.033f;
    static bool recordSimulation = false;

    // Simulation Control
    if (ImGui::CollapsingHeader("Simulation Control", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button(simulationRunning ? "Pause" : "Play"))
        {
            simulationRunning = !simulationRunning;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) 
        {
            simulationRunning = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Step")) {}

        ImGui::DragFloat("Simulation Speed", &simulationSpeed, 0.01f, 0.0f, 10.0f);
        ImGui::DragInt("Substeps", &substeps, 1, 1, 10);
    }

    // Time Settings
    if (ImGui::CollapsingHeader("Time Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Fixed Timestep", &fixedTimestep, 0.001f, 0.001f, 0.1f);
        ImGui::Checkbox("Enable Adaptive Timestep", &enableAdaptiveTimestep);
        if (enableAdaptiveTimestep)
        {
            ImGui::DragFloat("Max Timestep", &maxTimestep, 0.001f, 0.016f, 0.1f);
        }
    }

    // Collision Settings
    if (ImGui::CollapsingHeader("Collision Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Continuous Collision Detection", &enableContinuousCollision);
        static int collisionMethod = 0;
        ImGui::Combo("Collision Method", &collisionMethod, "Discrete\0Continuous\0Hybrid\0");
    }

    // Recording
    if (ImGui::CollapsingHeader("Recording", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Record Simulation", &recordSimulation);
        if (recordSimulation)
        {
            static char savePath[256] = "simulation_record.json";
            ImGui::InputText("Save Path", savePath, sizeof(savePath));
            if (ImGui::Button("Export Recording")) {}
            ImGui::SameLine();
            if (ImGui::Button("Clear Recording")) {}
        }
    }

    // Statistics
    if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Average FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Physics Time: %.2f ms", 16.0f); // Replace with actual physics time
        ImGui::Text("Collision Checks: %d", 1000);    // Replace with actual collision checks
        ImGui::Text("Active Bodies: %d", 100);        // Replace with actual body count
    }
}

static void ShowOptimizationPanel(HdEditorWindowData* windowData)
{
    static bool enableMultithreading = true;
    static bool enableGPUAcceleration = true;
    static int threadCount = 4;
    static float lodDistance = 100.0f;
    static bool enableOcclusionCulling = true;
    static bool enableInstancing = true;
    static bool enableBatchProcessing = true;
    static float cullingThreshold = 0.01f;

    if (ImGui::CollapsingHeader("Performance Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Enable Multithreading", &enableMultithreading);
        if (enableMultithreading)
        {
            ImGui::DragInt("Thread Count", &threadCount, 1, 1, 32);
        }
        ImGui::Checkbox("Enable GPU Acceleration", &enableGPUAcceleration);
    }

    if (ImGui::CollapsingHeader("LOD Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("LOD Distance", &lodDistance, 1.0f, 0.0f, 1000.0f);
        ImGui::Checkbox("Enable Occlusion Culling", &enableOcclusionCulling);
        if (enableOcclusionCulling)
        {
            ImGui::DragFloat("Culling Threshold", &cullingThreshold, 0.001f, 0.0f, 1.0f);
        }
    }

    if (ImGui::CollapsingHeader("Rendering Optimization", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Enable Instancing", &enableInstancing);
        ImGui::Checkbox("Enable Batch Processing", &enableBatchProcessing);
    }

    if (ImGui::CollapsingHeader("Memory Management", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static int poolSize = 1024;
        static bool enableMemoryPooling = true;
        static bool enableDynamicAllocation = false;

        ImGui::Checkbox("Enable Memory Pooling", &enableMemoryPooling);
        if (enableMemoryPooling)
        {
            ImGui::DragInt("Pool Size (MB)", &poolSize, 1, 64, 8192);
        }
        ImGui::Checkbox("Enable Dynamic Allocation", &enableDynamicAllocation);
    }

    if (ImGui::Button("Apply Optimization Settings"))
    {
        // Handle applying settings
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Reset to Defaults"))
    {
        // Handle resetting to defaults
    }
}

} // namespace hdImgui
