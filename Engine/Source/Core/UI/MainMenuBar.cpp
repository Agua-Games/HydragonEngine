/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MainMenuBar.h"
#include "hdImgui.h"

namespace hdImgui {
// === Layout variables ===

/**
 * @brief Show the main menu bar.
 * @param p_open A pointer to a boolean variable. If not NULL, the main menu bar will display a close button in the upper-right corner, and clicking it 
 * will set the variable to false.
 */
void ShowMainMenuBar(bool* p_open, HdEditorWindowData* windowData) {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent Project"))
            {
                ImGui::MenuItem("StGeorgeKnights.hdproj");
                ImGui::MenuItem("Kendo.hdproj");
                ImGui::MenuItem("SoulOfSaka.hdproj");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("ActionAdventureTemplate.hdproj");
                    ImGui::MenuItem("FightGameTemplate.hdproj"); 
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Close Project", "Ctrl+W")) {}
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save Project As..")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open SceneGraph", "Ctrl+Shift+O")) {}
            if (ImGui::MenuItem("Open Recent SceneGraph", "Ctrl+Shift+O")) {}
            if (ImGui::MenuItem("Close SceneGraph", "Ctrl+Shift+O")) {}
            if (ImGui::MenuItem("Save SceneGraph As..", "Ctrl+Shift+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open NodeGraph", "Ctrl+M")) {}
            if (ImGui::MenuItem("Open Recent NodeGraph", "Ctrl+M")) {}
            if (ImGui::MenuItem("Close NodeGraph", "Ctrl+M")) {}
            if (ImGui::MenuItem("Save NodeGraph As..", "Ctrl+M")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open Asset Manager", "Ctrl+A")) {}
            if (ImGui::MenuItem("Import Asset", "Ctrl+I")) {}
            if (ImGui::MenuItem("Export Asset", "Ctrl+E")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            if (ImGui::MenuItem("Procedural Paste", "CTRL+V")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Rename", "CTRL+R")) {}
            if (ImGui::MenuItem("Add Prefix", "CTRL+R")) {}
            if (ImGui::MenuItem("Add Suffix", "CTRL+R")) {}
            if (ImGui::MenuItem("Procedural Rename", "CTRL+R")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Delete", "CTRL+D")) {}
            if (ImGui::BeginMenu("Delete By Type")) 
            {   
                if (ImGui::MenuItem("Icons")){}
                if (ImGui::MenuItem("Gizmos")){}
                if (ImGui::MenuItem("Deformers")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Meshes")){}
                if (ImGui::MenuItem("Joints")){}
                if (ImGui::MenuItem("Lights")){}
                if (ImGui::MenuItem("Cameras")){}
                ImGui::Separator();
                if (ImGui::MenuItem("VDB Volumes")){}
                //if (ImGui::MenuItem("Alembic Volumes")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Points")){}
                if (ImGui::MenuItem("Particles")){}
                if (ImGui::MenuItem("Fields")){}
                if (ImGui::MenuItem("Simulation Boundaries")){}
                if (ImGui::MenuItem("Springs")){}
                if (ImGui::MenuItem("Rigid Bodies")){}
                if (ImGui::MenuItem("Dynamic Joints")){}
                if (ImGui::MenuItem("Dynamic Constraints")){}
                if (ImGui::MenuItem("Clothes")){}
                if (ImGui::MenuItem("Fluids")){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Delete All By Type")) 
            {   
                if (ImGui::MenuItem("Icons")){}
                if (ImGui::MenuItem("Gizmos")){}
                if (ImGui::MenuItem("Deformers")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Meshes")){}
                if (ImGui::MenuItem("Joints")){}
                if (ImGui::MenuItem("Lights")){}
                if (ImGui::MenuItem("Cameras")){}
                ImGui::Separator();
                if (ImGui::MenuItem("VDB Volumes")){}
                //if (ImGui::MenuItem("Alembic Volumes")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Points")){}
                if (ImGui::MenuItem("Particles")){}
                if (ImGui::MenuItem("Fields")){}
                if (ImGui::MenuItem("Simulation Boundaries")){}
                if (ImGui::MenuItem("Springs")){}
                if (ImGui::MenuItem("Rigid Bodies")){}
                if (ImGui::MenuItem("Dynamic Joints")){}
                if (ImGui::MenuItem("Dynamic Constraints")){}
                if (ImGui::MenuItem("Clothes")){}
                if (ImGui::MenuItem("Fluids")){}
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Frame Selected", "CTRL+F")) {}
            if (ImGui::MenuItem("Frame All", "CTRL+A")) {}
            if (ImGui::MenuItem("Lock Viewport to Selected", "CTRL+L")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Play Runtime", "CTRL+F")) {}
            if (ImGui::MenuItem("Pause Runtime", "CTRL+A")) {}
            if (ImGui::MenuItem("Step Runtime", "CTRL+L")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate", "CTRL+I")) {}
            if (ImGui::MenuItem("Procedural Duplicate", "CTRL+I")) {}
            if (ImGui::MenuItem("Duplicate With Transform", "CTRL+I")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Group", "CTRL+A")) {}
            if (ImGui::MenuItem("Ungroup", "CTRL+A")) {}
            if (ImGui::MenuItem("Create LOD NodeGraph", "CTRL+A")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Parent/Compose SceneGraph As Layer", "CTRL+A")) {}
            if (ImGui::MenuItem("Unparent SceneGraph", "CTRL+A")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Find", "CTRL+F")) {}   // Search in UI - menu item, command etc.
            if (ImGui::MenuItem("Explain", "CTRL+X")) {}   // Opens Agent to explain any UI element - its function, use etc.
            ImGui::Separator();
            if (ImGui::MenuItem("Keyboard Shortcuts")) {} 
            if (ImGui::MenuItem("Menus")) {} 
            if (ImGui::MenuItem("Project Settings")) {} 
            if (ImGui::MenuItem("Editor Preferences")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::BeginMenu("SceneGraph")) 
            {
                if (ImGui::MenuItem("Procedural SceneGraph")){}
                ImGui::Separator();
                if (ImGui::MenuItem("L-Tree")){}
                if (ImGui::MenuItem("Fractal")){}
                if (ImGui::MenuItem("Debris")){}     // a quickstart rig with some physics simulation, baking etc.
                if (ImGui::MenuItem("Raycast Rules")){}
                if (ImGui::MenuItem("Terrain Rules")){}
                if (ImGui::MenuItem("Spline Path")){}
                if (ImGui::MenuItem("Proc Building")){}
                if (ImGui::MenuItem("Proc Settlement")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Basic SceneGraph")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open SceneGraph Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("NodeGraph")) 
            {
                if (ImGui::MenuItem("Procedural NodeGraph")){}
                ImGui::Separator();
                if (ImGui::MenuItem("L-Tree")){}
                if (ImGui::MenuItem("Fractal")){}
                if (ImGui::MenuItem("Debris")){}     // a quickstart rig with some physics simulation, baking etc.
                if (ImGui::MenuItem("Raycast Rules")){}
                if (ImGui::MenuItem("Terrain Rules")){}
                if (ImGui::MenuItem("Spline Path")){}
                if (ImGui::MenuItem("Proc Building")){}
                if (ImGui::MenuItem("Proc Settlement")){}
                if (ImGui::MenuItem("Physics Destruction")){}
                if (ImGui::MenuItem("Physics Destructible")){}
                if (ImGui::MenuItem("Physics FX Field")){}
                if (ImGui::MenuItem("Physicalised Anim")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Proc Prop Rig")){}
                if (ImGui::MenuItem("Proc Weapon Rig")){}
                if (ImGui::MenuItem("Proc Character Rig")){}
                if (ImGui::MenuItem("Proc Vehicle Rig")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Proc Audio Source")){}
                if (ImGui::MenuItem("Proc Audio Listener")){}
                if (ImGui::MenuItem("Proc Audio Ambience")){}
                if (ImGui::MenuItem("Proc Audio Soundtrack")){}
                if (ImGui::MenuItem("Proc Character Audio")){}
                if (ImGui::MenuItem("Proc Audio Mixer")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Basic Animation")){}
                if (ImGui::MenuItem("Biped Character Anim")){}
                if (ImGui::MenuItem("Quadruped Character Anim")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Basic NodeGraph")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open NodeGraph Library")) {}
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Transform/Space Locator")){}
            ImGui::Separator();
            if (ImGui::BeginMenu("Curve Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Spline")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Spline L-Tree")){}
                if (ImGui::MenuItem("Spline Crawling")){}
                if (ImGui::MenuItem("Spline Fractal")){}
                if (ImGui::MenuItem("Spline Extrusion")){}
                if (ImGui::MenuItem("Spline")){}
                if (ImGui::MenuItem("Spline Rig")){}    // includes path constraints, optional physics etc. From roads to hanging cables, fences.
                if (ImGui::MenuItem("Spline Path")){}
                if (ImGui::MenuItem("Spline Cable")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Linear Spline")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Spline Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Mesh Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Mesh")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Box")){}
                if (ImGui::MenuItem("Sphere")){}
                if (ImGui::MenuItem("Capsule")){}
                if (ImGui::MenuItem("Cylinder")){}
                if (ImGui::MenuItem("Cone")){}
                if (ImGui::MenuItem("Plane")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Platonic Solid")){}
                if (ImGui::MenuItem("Manifold Place")){}
                if (ImGui::MenuItem("Manifold Solid")){}
                if (ImGui::MenuItem("Pyramid")){}
                if (ImGui::MenuItem("Prism")){}
                if (ImGui::MenuItem("Pipe")){}
                if (ImGui::MenuItem("Helix")){}
                if (ImGui::MenuItem("Gear")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Mesh Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Volume Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Volume")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Voxels Array Structure")){}
                if (ImGui::MenuItem("Voxels Rock")){}
                if (ImGui::MenuItem("Voxels Cliff")){}
                if (ImGui::MenuItem("Voxels Debris")){}
                if (ImGui::MenuItem("Voxels Cave")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Box")){}
                if (ImGui::MenuItem("Sphere")){}
                if (ImGui::MenuItem("Capsule")){}
                if (ImGui::MenuItem("Cylinder")){}
                if (ImGui::MenuItem("Cone")){}
                if (ImGui::MenuItem("Plane")){}     // for 2D fluid sim, sliced sim, for example.
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Volume Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Light Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Light")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Ambient")){}
                if (ImGui::MenuItem("Directional")){}
                if (ImGui::MenuItem("Point")){}
                if (ImGui::MenuItem("Spot")){}
                if (ImGui::MenuItem("Area")){}
                if (ImGui::MenuItem("Volume")){}     // for 2D fluid sim, sliced sim, for example.
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Light Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Camera Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Camera")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Camera & Cam Rig")){}
                if (ImGui::MenuItem("Camera & Light Rig")){}
                if (ImGui::MenuItem("Camera Film Set")){}
                if (ImGui::MenuItem("Physics Action Camera")){}
                if (ImGui::MenuItem("Physics Vehicle Camera")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Orthographic")){}
                if (ImGui::MenuItem("Perspective")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Camera Library")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Texture Primitive")) 
            {
                if (ImGui::MenuItem("Procedural Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Nature Ground Texture")) {}
                if (ImGui::MenuItem("Snow Texture")) {}
                if (ImGui::MenuItem("Ice Texture")) {}
                if (ImGui::MenuItem("Rock Texture")) {}
                if (ImGui::MenuItem("Cliff Texture")) {}
                if (ImGui::MenuItem("Bark Texture")) {}
                if (ImGui::MenuItem("Branch Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Architectural Ground Texture")) {}
                if (ImGui::MenuItem("Architectural Wall Texture")) {}
                if (ImGui::MenuItem("Architectural Roof Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Aged Metal Texture")) {}
                if (ImGui::MenuItem("Aged Armor Texture")) {}
                if (ImGui::MenuItem("Aged Fabric Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Skin Texture")) {}
                if (ImGui::MenuItem("Hair Texture")) {}
                if (ImGui::MenuItem("Fur Texture")) {}
                if (ImGui::MenuItem("Eye Texture")) {}
                if (ImGui::MenuItem("Scales Skin Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Render Target/Texture")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Texture Library")) {}
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Utilities")) 
            {
                if (ImGui::MenuItem("Measure Distance")){}
                if (ImGui::MenuItem("Arc Length")){}
                if (ImGui::MenuItem("SceneGraph Note")){}
                if (ImGui::MenuItem("Construction Plane")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Proc Utility Library")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Generate"))
        {
            if (ImGui::MenuItem("Call Agent", "Ctrl+Shift+N")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("NodeGraph", "Ctrl+Shift+N")) {}
            if (ImGui::MenuItem("SceneGraph", "Ctrl+Shift+N")) {}
            if (ImGui::MenuItem("Script", "Ctrl+Shift+S")) {}
            if (ImGui::MenuItem("Procedural", "Ctrl+P")) {}
            if (ImGui::MenuItem("Macro", "Ctrl+Shift+M")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Image", "Ctrl+Shift+I")) {}        
            if (ImGui::MenuItem("Mesh", "Ctrl+Shift+I")) {}        
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Select"))
        {
            if (ImGui::MenuItem("All")) {}
            if (ImGui::BeginMenu("All By Type")) 
            {   
                if (ImGui::MenuItem("Icons")){}
                if (ImGui::MenuItem("Gizmos")){}
                if (ImGui::MenuItem("Deformers")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Meshes")){}
                if (ImGui::MenuItem("Joints")){}
                if (ImGui::MenuItem("Lights")){}
                if (ImGui::MenuItem("Cameras")){}
                ImGui::Separator();
                if (ImGui::MenuItem("VDB Volumes")){}
                //if (ImGui::MenuItem("Alembic Volumes")){}
                ImGui::Separator();
                if (ImGui::MenuItem("Points")){}
                if (ImGui::MenuItem("Particles")){}
                if (ImGui::MenuItem("Fields")){}
                if (ImGui::MenuItem("Simulation Boundaries")){}
                if (ImGui::MenuItem("Springs")){}
                if (ImGui::MenuItem("Rigid Bodies")){}
                if (ImGui::MenuItem("Dynamic Joints")){}
                if (ImGui::MenuItem("Dynamic Constraints")){}
                if (ImGui::MenuItem("Clothes")){}
                if (ImGui::MenuItem("Fluids")){}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {}
            if (ImGui::MenuItem("SceneGraph Hierarchy", "Ctrl+H")) {}
            if (ImGui::MenuItem("SceneGraph Children", "Ctrl+H")) {}
            if (ImGui::MenuItem("SceneGraph Root", "Ctrl+H")) {}
            if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {}
            if (ImGui::MenuItem("Similar", "Ctrl+S")) {}
            if (ImGui::MenuItem("Selection Sets", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Grow", ">")) {}
            if (ImGui::MenuItem("Shrink", "<")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Object/Component", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Polygon Components")) 
            {
                if (ImGui::MenuItem("Vertex")){}
                if (ImGui::MenuItem("Edge")){}
                if (ImGui::MenuItem("Face")){}
                if (ImGui::MenuItem("UV")){}
                if (ImGui::MenuItem("UV Shell")){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Volume Components")) 
            {
                if (ImGui::MenuItem("Voxel")){}
                if (ImGui::MenuItem("Boundaries")){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Physics Components")) 
            {
                if (ImGui::MenuItem("Particle/Point")){}
                if (ImGui::MenuItem("Emitter")){}
                if (ImGui::MenuItem("Spring")){}
                if (ImGui::MenuItem("Rigid Body")){}
                if (ImGui::MenuItem("Dynamic Joint")){}
                if (ImGui::MenuItem("Dynamic Constraint")){}
                if (ImGui::MenuItem("Cloth Mesh")){}
                if (ImGui::MenuItem("Fluid")){}
                if (ImGui::MenuItem("Destructible Collection")){}
                if (ImGui::MenuItem("Destructible Chunk")){}
                if (ImGui::MenuItem("Collider")){}
                if (ImGui::MenuItem("Field")){}
                if (ImGui::MenuItem("Simulation Boundary")){}
                if (ImGui::MenuItem("System")){}
                if (ImGui::MenuItem("Solver & Assigned")){}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Asset"))
        {
            if (ImGui::MenuItem("Import", "Ctrl+I")) {}
            if (ImGui::MenuItem("Export", "Ctrl+E")) {}
            if (ImGui::MenuItem("Reimport", "Ctrl+R")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Procedural Asset Batch Processor", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open Asset Library", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("NodeGraph"))
        {  
            if (ImGui::MenuItem("Create Node", "Ctrl+N")) {}
            if (ImGui::MenuItem("Delete Node", "Ctrl+Del")) {}
            if (ImGui::MenuItem("Rename Node", "Ctrl+R")) {}
            if (ImGui::MenuItem("Duplicate Node", "Ctrl+D")) {}
            if (ImGui::MenuItem("Replace Selected Node", "Ctrl+D")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open NodeGraph Library", "Ctrl+D")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("SceneGraph"))
        {
            if (ImGui::MenuItem("Create Node", "Ctrl+N")) {}
            if (ImGui::MenuItem("Delete Node", "Ctrl+Del")) {}
            if (ImGui::MenuItem("Rename Node", "Ctrl+R")) {}
            if (ImGui::MenuItem("Duplicate Node", "Ctrl+D")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Replace Selected Node", "Ctrl+D")) {}
            if (ImGui::MenuItem("Override Selected Instance", "Ctrl+D")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open SceneGraph Library", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Rendering"))
        {
            static int s_lightingType = 3;
            if (ImGui::Combo("Lighting Type", &s_lightingType, "No Lighting\0Lighting Only\0Lighting\0Lighting + Detail")) {}
            ImGui::Separator();
            static bool s_enableGI = true;
            static bool s_enableSSAO = true;
            static bool s_enableSkylight = true;
            static bool s_enableAtmosphere = true;
            static bool s_enableClouds = true;
            static int s_cloudsMethod = 0;
            static bool s_enableFog = true;
            static int s_fogMethod = 0;
            static bool s_enableShadows = true;
            static int s_shadowMethod = 0;
            if (ImGui::Checkbox("Enable Shadows", &s_enableShadows)) {}
            if (ImGui::Combo("Shadows Method", &s_shadowMethod, "Depth Map Shadows\0Virtual Shadows Simple\0Virtual Shadows Advanced")) {}
            if (ImGui::Checkbox("Enable GI", &s_enableGI)) {}
            if (ImGui::Checkbox("Enable SSAO", &s_enableSSAO)) {}
            if (ImGui::Checkbox("Enable Skylight", &s_enableSkylight)) {}
            if (ImGui::Checkbox("Enable Atmosphere", &s_enableAtmosphere)) {}
            if (ImGui::Checkbox("Enable Clouds", &s_enableClouds)) {}
            if (ImGui::Combo("Clouds Method", &s_cloudsMethod, "Flat Layer\0Basic Volumetric\0Detailed Volumetric", 3)) {}
            if (ImGui::Checkbox("Enable Fog", &s_enableFog)) {}
            if (ImGui::Combo("Fog Type", &s_fogMethod, "Simple Depth-based\0Basic Volumetric\0Detailed Volumetric", 3)) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Lighting Editor", "Ctrl+O")) {}      
            if (ImGui::MenuItem("Lighting Profiler", "Ctrl+O")) {}      // Opens the Lighting tab in the Profiler Editor
            if (ImGui::MenuItem("Rendering Settings", "Ctrl+O")) {} 
            ImGui::Separator();
            if (ImGui::MenuItem("Open Lighting Library", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Physics"))
        {
            static bool s_enablePhysics = true;
            static bool s_enableLocalDetail = true;
            static bool s_enablePhysicalisedAnimation = true;
            static int s_PhysicsSolverType = 0;
            static int s_PhysLocalDetailType = 0;
            static int s_PhysAnimType = 0;
            if (ImGui::Checkbox("Enable Physics", &s_enablePhysics)) {}
            if (ImGui::Checkbox("Enable Local Detail", &s_enablePhysics)) {}
            if (ImGui::Checkbox("Enable Physicalised Animation", &s_enablePhysicalisedAnimation)) {}
            ImGui::Separator();
            if (ImGui::Combo("Physics Solver Type", &s_PhysicsSolverType, "Standard Solver\0Fast Solver\0GPU-Compute Solver")) {}
            if (ImGui::Combo("Local Detail Solver Type", &s_PhysLocalDetailType, "Standard Solver\0Fast Solver\0GPU-Compute Solver")) {}
            if (ImGui::Combo("Physicalized Animation Type", &s_PhysAnimType, "Standard PhysAnim\0Fast PhysAnim\0GPU-Compute PhysAnim")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Proc Prop Physics Rig", "Ctrl+O")) {}
            if (ImGui::MenuItem("Proc Destructible Physics Rig", "Ctrl+O")) {}
            if (ImGui::MenuItem("Proc Character Physics Rig", "Ctrl+O")) {}
            if (ImGui::MenuItem("Proc Vehicle Physics Rig", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Add Procedural Physics Rig to SceneGraph", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Physics Editor", "Ctrl+O")) {}
            if (ImGui::MenuItem("Physics Profiler", "Ctrl+O")) {}
            if (ImGui::MenuItem("Physics Settings", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open Physics Library", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Run"))
        {
            if (ImGui::MenuItem("Open Script", "Ctrl+O")) {}
            if (ImGui::MenuItem("Run Script", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Open Editor Scripts Library", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            static bool s_showGrid = true;
            if (ImGui::Checkbox("Grid", &s_showGrid)) {}
            if (ImGui::MenuItem("Contextual Info Overlay", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Hide")) 
            {   if (ImGui::MenuItem("Hide Selected")){}
                    if (ImGui::MenuItem("Hide Unselected")){}
                    if (ImGui::MenuItem("All")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Hide Icons")){}
                    if (ImGui::MenuItem("Hide Gizmos")){}
                    if (ImGui::MenuItem("Hide Deformers")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Hide Meshes")){}
                    if (ImGui::MenuItem("Hide Joints")){}
                    if (ImGui::MenuItem("Hide Lights")){}
                    if (ImGui::MenuItem("Hide Cameras")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Hide VDB Volumes")){}
                    //if (ImGui::MenuItem("Hide Alembic Volumes")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Hide Points")){}
                    if (ImGui::MenuItem("Hide Particles")){}
                    if (ImGui::MenuItem("Hide Fields")){}
                    if (ImGui::MenuItem("Hide Simulation Boundaries")){}
                    if (ImGui::MenuItem("Hide Springs")){}
                    if (ImGui::MenuItem("Hide Colliders")){}
                    if (ImGui::MenuItem("Hide Rigid Bodies")){}
                    if (ImGui::MenuItem("Hide Dynamic Joints")){}
                    if (ImGui::MenuItem("Hide Dynamic Constraints")){}
                    if (ImGui::MenuItem("Hide Clothes")){}
                    if (ImGui::MenuItem("Hide Fluids")){}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Show"))
            {   if (ImGui::MenuItem("Show Selected")){}
                    if (ImGui::MenuItem("Show Unselected")){}
                    if (ImGui::MenuItem("All")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Show Icons")){}
                    if (ImGui::MenuItem("Show Gizmos")){}
                    if (ImGui::MenuItem("Show Deformers")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Show Meshes")){}
                    if (ImGui::MenuItem("Show Joints")){}
                    if (ImGui::MenuItem("Show Lights")){}
                    if (ImGui::MenuItem("Show Cameras")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Show VDB Volumes")){}
                    //if (ImGui::MenuItem("Show Alembic Volumes")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Show Points")){}
                    if (ImGui::MenuItem("Show Particles")){}
                    if (ImGui::MenuItem("Show Fields")){}
                    if (ImGui::MenuItem("Show Simulation Boundaries")){}
                    if (ImGui::MenuItem("Show Springs")){}
                    if (ImGui::MenuItem("Show Colliders")){}
                    if (ImGui::MenuItem("Show Rigid Bodies")){}
                    if (ImGui::MenuItem("Show Dynamic Joints")){}
                    if (ImGui::MenuItem("Show Dynamic Constraints")){}
                    if (ImGui::MenuItem("Show Clothes")){}
                    if (ImGui::MenuItem("Show Fluids")){}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Top Toolbar", "Ctrl+I")) { windowData->isTopToolbarWindowOpen = true; }
            if (ImGui::MenuItem("Left Toolbar", "Ctrl+I")) { windowData->isLeftToolbarWindowOpen = true; }
            if (ImGui::MenuItem("Right Toolbar", "Ctrl+I")) { windowData->isRightToolbarWindowOpen = true; }
            if (ImGui::MenuItem("Bottom Toolbar", "Ctrl+I")) { windowData->isBottomToolbarWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("NodeGraph", "Ctrl+M")) { windowData->isNodeGraphWindowOpen = true; }
            if (ImGui::MenuItem("SceneGraph", "Ctrl+O")) { windowData->isSceneGraphWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("Viewport 3D", "Ctrl+Shift+V")) { windowData->isViewport3DWindowOpen = true; }
            if (ImGui::MenuItem("Viewport 3D Tools", "Ctrl+Shift+T")) {windowData->isViewport3DToolsWindowOpen = true; }
            if (ImGui::MenuItem("Viewport 2D", "Ctrl+Shift+D")) { windowData->isViewport2DWindowOpen = true; }
            if (ImGui::MenuItem("Viewport 2D Tools", "Ctrl+Shift+O")) { windowData->isViewport2DToolsWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("Asset Manager", "Ctrl+A")) { windowData->isAssetManagerWindowOpen = true; }
            if (ImGui::MenuItem("Chimera Pipeline", "Ctrl+Shift+C")) { windowData->isChimeraPipelineWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("Agents", "Ctrl+Shift+A")) { windowData->isAgentsWindowOpen = true; }        
            if (ImGui::MenuItem("AITask", "Ctrl+Shift+A")) { windowData->isAITaskWindowOpen = true; }        
            ImGui::Separator();  
            if (ImGui::MenuItem("Properties", "Ctrl+Shift+P")) { windowData->isPropertiesWindowOpen = true; }       
            if (ImGui::MenuItem("Settings", "Ctrl+Shift+P")) { windowData->isSettingsWindowOpen = true; }
            ImGui::Separator();  
            if (ImGui::MenuItem("File Explorer", "Ctrl+Shift+M")) { windowData->isFileExplorerWindowOpen = true; }
            if (ImGui::MenuItem("Lighting Editor", "Ctrl+Shift+M")) { windowData->isLightingWindowOpen = true; }
            if (ImGui::MenuItem("Physics Editor", "Ctrl+Shift+M")) { windowData->isPhysicsWindowOpen = true; }
            if (ImGui::MenuItem("Image Tools", "Ctrl+Shift+M")) { windowData->isImageWindowOpen = true; }
            if (ImGui::MenuItem("Mesh Editor", "Ctrl+Shift+M")) { windowData->isMeshWindowOpen = true; }
            if (ImGui::MenuItem("Volume Editor", "Ctrl+Shift+V")) { windowData->isVolumeWindowOpen = true; }
            if (ImGui::MenuItem("Montage Editor", "Ctrl+Shift+T")) { windowData->isMontageWindowOpen = true; }
            if (ImGui::MenuItem("Audio Editor", "Ctrl+Shift+A")) { windowData->isAudioWindowOpen = true; }
            if (ImGui::MenuItem("Drama Editor", "Ctrl+Shift+D")) { windowData->isDramaWindowOpen = true; }
            if (ImGui::MenuItem("Pattern Orchestrator", "Ctrl+Shift+D")) { windowData->isPatternOrchestratorWindowOpen = true; }
            if (ImGui::MenuItem("Streaming Editor", "Ctrl+Shift+M")) { windowData->isStreamingWindowOpen = true; }
            if (ImGui::MenuItem("Networking Editor", "Ctrl+Shift+N")) { windowData->isNetworkingWindowOpen = true; }
            if (ImGui::MenuItem("Input Editor", "Ctrl+Shift+T")) { windowData->isInputWindowOpen = true; }
            if (ImGui::MenuItem("Localization Editor", "Ctrl+Shift+E")) { windowData->isLocalizationWindowOpen = true; }
            if (ImGui::MenuItem("Properties Matrix", "Ctrl+Shift+E")) { windowData->isPropertiesMatrixWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("UI Editor", "Ctrl+Shift+U")) { windowData->isUIEditorWindowOpen = true; }
            if (ImGui::MenuItem("Font Editor", "Ctrl+Shift+F")) { windowData->isFontWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::MenuItem("Scripts Palette", "Ctrl+Shift+P")) { windowData->isScriptsPaletteWindowOpen = true; }
            if (ImGui::MenuItem("Command Palette", "Ctrl+Shift+P")) { windowData->isCommandsPaletteWindowOpen = true; }
            if (ImGui::MenuItem("Console", "Ctrl+Shift+L")) { windowData->isConsoleWindowOpen = true; }
            if (ImGui::MenuItem("Bottom Status Bar", "Ctrl+Shift+L")) { windowData->isBottomStatusBarWindowOpen = true; }
            ImGui::Separator();
            if (ImGui::BeginMenu("Developer"))
            {
                if (ImGui::MenuItem("Macros Editor", "Ctrl+Shift+M")) { windowData->isMacrosWindowOpen = true; }
                if (ImGui::MenuItem("Script Editor", "Ctrl+Shift+O")) { windowData->isScriptWindowOpen = true; }
                if (ImGui::MenuItem("Reflection Editor", "Ctrl+Shift+R")) { windowData->isReflectionWindowOpen = true; }
                if (ImGui::MenuItem("Plugin", "Ctrl+Shift+L")) { windowData->isPluginWindowOpen = true; }
                if (ImGui::MenuItem("Extensions", "Ctrl+Shift+E")) { windowData->isExtensionsWindowOpen = true; }
                ImGui::Separator();
                if (ImGui::MenuItem("Collaboration Editor", "Ctrl+Shift+E")) { windowData->isCollaborationWindowOpen = true; }
                if (ImGui::MenuItem("Community Editor", "Ctrl+Shift+E")) { windowData->isCommunityWindowOpen = true; }
                if (ImGui::MenuItem("Monetization Editor", "Ctrl+Shift+E")) { windowData->isMonetizationWindowOpen = true; }
                ImGui::Separator();
                if (ImGui::MenuItem("Performance Scalability", "Ctrl+Shift+B")) { windowData->isPerformanceScalabilityWindowOpen = true; }
                if (ImGui::MenuItem("Project Insights", "Ctrl+Shift+B")) { windowData->isProjectInsightsWindowOpen = true; }
                ImGui::Separator();
                if (ImGui::MenuItem("Profiler", "Ctrl+Shift+P")) { windowData->isProfilerWindowOpen = true; }
                if (ImGui::MenuItem("Debugger", "Ctrl+Shift+D")) { windowData->isDebugWindowOpen = true; }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Hydragon Documentation", "F1")) {}
            if (ImGui::MenuItem("What's New")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Intro Tutorials")) {}
            if (ImGui::MenuItem("Learn", "F3")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Support Center")) {}
            if (ImGui::MenuItem("Report a Problem")) {}
            if (ImGui::MenuItem("Contribute")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("About Hydragon")) {}
            if (ImGui::MenuItem("Hydragon Home Page")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
} //  namespace hdImgui