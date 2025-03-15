/**
* Copyright (c) 2025 Agua Games. All rights reserved.
* Licensed under the Agua Games License 1.0
*
* @file Engine.h
* @brief Entry point for the Engine Core.
* (The Editor is a toggleable embedded layer for the engine).
* 
* ARCHITECTURAL NOTES:
* - The Engine class serves as the central coordinator for all major systems
* - It implements a hybrid architecture combining:
*   1. Node-based processing (primary)
*   2. Traditional scripting
*   3. Natural language interfaces
* - Systems are designed to work both independently and in harmony
* - Procedural generation is deeply integrated at all levels
* - The architecture supports both active guidance and passive evolution
* 
* DESIGN PRINCIPLES:
* - Everything is a node in the system
* - Systems can self-organize and evolve
* - Users can intervene at any level of abstraction
* - Harmony is maintained automatically unless explicitly overridden
* - Performance is optimized through intelligent task distribution
* 
* TODO (Sketch Phase):
* - Implement core node graph architecture
* - Setup basic orchestration systems
* - Integrate procedural generation framework
* - Establish communication protocols between systems
* - Create initial harmony maintenance systems
*/
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "NodeGraph.h"                   // Foundation
#include "ProceduralOrchestrator.h"      // Evolution
#include "SystemOrchestrator.h"             // Coordination
#include "ResourceManager.h"                // Assets
#include "VulkanBackend.h"                  // Graphics

namespace hd {

class Engine {
public:
    // Initialization and shutdown
    static Engine& GetInstance();
    bool Initialize();
    void Shutdown();
    
    // Core system access
    NodeGraph& GetNodeGraph();
    ProceduralOrchestrator& GetProceduralOrchestrator();
    SystemOrchestrator& GetSystemOrchestrator();
    
    // Runtime control
    void Update(float deltaTime);
    void ProcessSystems();
    
    // System configuration
    void SetSystemIntent(const SystemIntent& intent);
    void ConfigureHarmonyParameters(const HarmonyParams& params);
    
    // Resource management
    ResourceManager& GetResourceManager();
    
    // Debug and development
    void EnableDebugMode(bool enable);
    void SetProfilerLevel(ProfilerLevel level);

private:
    Engine() = default;
    ~Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Core systems
    std::unique_ptr<NodeGraph> m_nodeGraph;
    std::unique_ptr<ProceduralOrchestrator> m_proceduralOrchestrator;
    std::unique_ptr<SystemOrchestrator> m_systemOrchestrator;
    std::unique_ptr<ResourceManager> m_resourceManager;
    
    // Graphics backend
    std::unique_ptr<Graphics::VulkanBackend> m_graphicsBackend;
    
    /**
     * @brief Internal state of the engine.
     * Tracks the state of the engine and its systems, like system health, resource usage, 
     * evolution progress, and harmony metrics.
     */
    struct EngineState {
        bool isInitialized = false;
        bool isDebugMode = false;
        ProfilerLevel profilerLevel = ProfilerLevel::Basic;
        SystemState currentState;      // Holds collective state of all systems
        HarmonyMetrics harmonyMetrics; // Metrics for system harmony
    } m_state;
    
    // Internal methods
    void InitializeSystems();
    void SetupDefaultConfiguration();
    void UpdateHarmonyMetrics();
    void ProcessSystemIntents();
    void MaintainSystemHarmony();
};

} // namespace hd
