/**
* Copyright (c) 2025 Agua Games. All rights reserved.
* Licensed under the Agua Games License 1.0
*
* @file Engine.h
* @brief Entry point for the Engine Core.
* (The Editor is a toggleable embedded layer for the engine).
* 
* ARCHITECTURAL NOTES:
* - The Engine class serves as the central coordinator for all major systems, including the editor.
* - It implements a synthetic architecture, where:
*   1. Node-based processing is the core of the system, with:
*     - Traditional scripting as the primary means of interaction
*     - Visual scripting as an alternative means of interaction
*   2. Natural language interfaces as a means of interaction
* - Systems are designed to work both independently and in harmony.
* - The architecture supports both active guidance and passive evolution.
* - Asynchronous streaming and multi-threading are integral.
* 
* DESIGN PRINCIPLES:
* - Everything is a node in the system
* - Engine core and UI are totally decoupled. Users can have a productive and pleasing coding experience, where they do most things in code alone.
* Features like runtime editing of variable values without recompilation, runtime compilation of scripts, execution of single commands, coroutines, etc. are possible.
* - Procedural generation is deeply integrated at all levels
* - Systems can self-organize and evolve
* - Users can intervene at any level of abstraction
* - Harmony is maintained automatically unless explicitly overridden
* - Performance is optimized through intelligent task distribution
* 
* TODO (Sketch Phase):
* @todo Move implementation to .cpp file
* @todo (I need to study this design choice better) including the system itself
* @todo Establish communication protocols between systems
*/
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Node.h"                          // Foundational
#include "NodeManager.h"                   // NodeGraph
#include "PhysicsManager.h"                // Physics, etc.
#include "AudioManager.h"                  // Audio, etc.
#include "GameplayManager.h"               // Gameplay, etc.
#include "ResourceManager.h"               // Assets
#include "Renderer.h"                      // Graphics, Rendering, etc.
#include "InputManager.h"                  // Input, UI, etc.
#include "TimeManager.h"                   // Time, Profiling, etc.
#include "ProceduralManager.h"             // Coordination, Harmony, etc.
#include "MainEditorUIManager.h"                     // UI, Editor, etc.
#include "EngineTypes.h"                   // Engine modes, etc.

namespace hd {

/**
 * @brief The main engine class.
 */
class Engine {
public:
    // === Structure Definitions ===
    /**
     * @brief Internal state of the engine.
     * Tracks the state of the engine and its systems, like system health, resource usage, 
     * evolution progress, and harmony metrics.
     */
    struct EngineState {
        bool isInitialized = false;
        bool isDebugMode = false;
        ProfilerLevel profilerLevel = ProfilerLevel::Basic;
        SystemState currentState;                           // Holds collective state of all systems
        HarmonyMetrics harmonyMetrics;                      // Metrics for system harmony
    };

    // === Resource Management ===
    
    // === Allocation, Initialization, Loading ===
    static Engine& getInstance();
    bool initialize();

    bool shouldClose = false;
    EngineMode mode = EngineMode::FULL;     // Engine mode, like GAMEPLAY_ONLY, EDITOR_ONLY, etc.

    // Core systems access - for internal use only. Use NodeManager for public access.
    ProceduralManager& getProceduralManager();
    NodeManager& getNodeManager();
    
    // === Processing ===
    // Runtime control
    void update(float deltaTime);
    EngineMode getMode() const;
    void setMode(EngineMode mode);                 // Set the engine mode, like gameplay, editor, etc.
    
    /**
     * @brief The main loop of the engine.
     */
    void mainLoop() {
        // Same core loop for both game and editor
        while (!shouldClose) {
            // Managers always present, just controlled by mode
            resourceManager->update();
            inputManager->processInput();
            timeManager->update();
            proceduralManager->update();
            nodeManager->update();
            physicsManager->update();
            audioManager->update();
            gameplayManager->update();
            MainEditorUIManager->update();                    // Editor UI not processed in GAMEPLAY_ONLY. SELECTIVE mode may disable some UI.
            renderer->update();
            // ... Add more systems here, as needed.
        }
    }

    // === Debugging/Development ===
    void enableDebugMode(bool enable);
    void setProfilerLevel(ProfilerLevel level);

    // === Cleanup ===
    void shutdown();
    ~Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    // === Allocation, Initialization, Loading ===
    Engine() = default;

    // Core systems
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<TimeManager> timeManager;
    std::unique_ptr<ProceduralManager> proceduralManager;
    std::unique_ptr<NodeManager> nodeManager;
    std::unique_ptr<PhysicsManager> physicsManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<GameplayManager> gameplayManager;
    std::unique_ptr<MainEditorUIManager> MainEditorUIManager;
    std::unique_ptr<EngineState> state;     // Internal state of the engine. System health, resource usage, evolution progress, harmony metrics.

    // Internal methods
    void initializeSystems() {
        // Initialize all systems here
        renderer = std::make_unique<Renderer>();
        resourceManager = std::make_unique<ResourceManager>();
        inputManager = std::make_unique<InputManager>();
        timeManager = std::make_unique<TimeManager>();
        proceduralManager = std::make_unique<ProceduralManager>();
        nodeManager = std::make_unique<NodeManager>();
        physicsManager = std::make_unique<PhysicsManager>();
        audioManager = std::make_unique<AudioManager>();
        gameplayManager = std::make_unique<GameplayManager>();
        MainEditorUIManager = std::make_unique<MainEditorUIManager>();
        state = std::make_unique<EngineState>();

    }
    void setupDefaultConfiguration();
}

} // namespace hd

#endif