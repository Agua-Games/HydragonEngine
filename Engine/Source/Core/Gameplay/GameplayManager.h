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
#include "ScoreManager.h"

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
    GameplayData.GameplayMode gameplayMode = GameplayData.GameplayMode::Custom;   // Default gameplay mode is Custom.
    GameplayData.State gameplayState;
    EngineMode engineMode = Mode::GAME_ONLY;                  // Default mode is gameplay enabled, editor disabled, overlays disabled
    std::vector<std::shared_ptr<Node>> m_subsystems;          // Subsystems, like audio, physics, etc. that are managed by the GameplayManager.
    ScoreManager scoreManager;                                // Score manager for gameplay.
    gameplayData.maxPlayers = 10;
    gameplayData.maxSpawns = 10;
    gameplayData.respawnTime = 5.0f;
    gameplayData.timeLimit = 600.0f;
    gameplayData.maxSpectators = 10;
    gameplayData.overtime = false;
    gameplayData.killCam = false;
    gameplayData.classLimits = false;

    scoreManager.initialize();

    // === Processing ===
    void processNode() override {
        if (!m_active) return;

        // Process inputs, based on mode
        auto mode = getInputValue<Mode>("mode");                                        // Process the other inputs or not, based on mode.
        gameplaySettings = getInputValue<GameplayData.Settings>("gameplaySettings");    // Process the other inputs or not, based on mode.
        gameplayData = getInputValue<GameplayData>("gameplayData");                     // Process the other inputs or not, based on mode.
        gameplayState = getInputValue<GameplayData.State>("gameplayState");             // Process the other inputs or not, based on mode.

        // Adjust to Engine Mode - disable gameplay, etc. based on mode.
        adjustToGameplayMode(mode);

        // Process gameplay
        auto gameplayStatus = updateGameplay(gameplaySettings, gameplayData, gameplayState);

        // Set outputs
        setOutputValue("gameplayStatus", gameplayStatus);
        setOutputValue("gameplayMetrics", computeGameplayMetrics(gameplayStatus));
    }
    void addGameplayMode(Mode mode);
    void removeGameplayMode(Mode mode);
    void setGameplayMode(Mode mode) {
        gameplayMode = mode;
        // Adjust subsystems but maintain core functionality
        for (auto& subsystem : m_subsystems) {
            subsystem->adjustToGameplayMode(mode);    // Adjust to the given mode, like disabling gameplay, etc. based on mode.
        }
    }
    void adjustToGameplayMode(Mode mode) override;    // Adjust to the given mode, like disabling gameplay, etc. based on mode.
    void addGameplayRuleset(GameplayData::GameplayRules ruleset);
    void removeGameplayRuleset(GameplayData::GameplayRules ruleset);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayManager() = default;     // Default destructor
};

} // namespace hd
