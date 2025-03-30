/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayManager.h
 * @brief Header file for the GameplayManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayManager is a singleton class that manages the gameplay in the game.
 * - It is responsible for updating the gameplay and managing the gameplay system.
 * - It uses the Vulkan API for gameplay management.
 */
#pragma once
#include "NodeGraph/Node.h"
#include "EngineTypes.h"
#include "GameplayData.h"

namespace hd {

struct GameplayManagerInfo : public NodeInfo {
    GameplayManagerInfo() {
        nodeType = "Gameplay/GameplayManager";
        inputs = {
            "gameplaySettings",  // Gameplay settings 
            "gameplayData",      // Gameplay data
            "gameplayState"      // Gameplay state
        };
        outputs = {
            "gameplayStatus",  // Gameplay status
            "gameplayMetrics"  // Gameplay performance metrics
        };
    }
};

class GameplayManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit GameplayManager(const GameplayManagerInfo& info = GameplayManagerInfo())
        : Node(info) {}    // Default constructor for GameplayManager class
    initialize() override {}
    load() override {}

    // Set default values
    GameplayData gameplayData;
    GameplayData.Settings gameplaySettings;
    GameplayData.GlobalFlags gameplayGlobalFlags;
    GameplayData.LocalFlags gameplayLocalFlags;
    GameplayData.Stats gameplayStats;
    GameplayData.Mode gameplayMode = GameplayData.Mode::Custom;
    GameplayData.State gameplayState;
    EngineMode engineMode = Mode::GAME_ONLY;                      // Default mode is gameplay enabled, editor disabled, overlays disabled
    std::vector<std::shared_ptr<Node>> m_subsystems;          // Subsystems, like audio, physics, etc. that are managed by the GameplayManager.

    // === Processing ===
    void processNode() override {
        if (!m_active) return;

        // Process inputs, based on mode
        auto mode = getInputValue<Mode>("mode");                                        // Process the other inputs or not, based on mode.
        gameplaySettings = getInputValue<GameplayData.Settings>("gameplaySettings");    // Process the other inputs or not, based on mode.
        gameplayData = getInputValue<GameplayData>("gameplayData");                     // Process the other inputs or not, based on mode.
        gameplayState = getInputValue<GameplayData.State>("gameplayState");             // Process the other inputs or not, based on mode.

        // Adjust to Engine Mode - disable gameplay, etc. based on mode.
        adjustToMode(mode);

        // Process gameplay
        auto gameplayStatus = updateGameplay(gameplaySettings, gameplayData, gameplayState);

        // Set outputs
        setOutputValue("gameplayStatus", gameplayStatus);
        setOutputValue("gameplayMetrics", computeGameplayMetrics(gameplayStatus));
    }
    void addMode(Mode mode);
    void removeMode(Mode mode);
    void setMode(Mode mode) {
        m_mode = mode;
        // Adjust subsystems but maintain core functionality
        for (auto& subsystem : m_subsystems) {
            subsystem->adjustToMode(mode);
        }
    }
    void adjustToMode(Mode mode) override;    // Adjust to the given mode, like disabling gameplay, etc. based on mode.
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayManager() = default;     // Default destructor
};

} // namespace hd
