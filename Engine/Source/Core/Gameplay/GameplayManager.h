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
    GameplaySettings gameplaySettings;
    GameplayData gameplayData;
    GameplayState gameplayState;

    // === Processing ===
    void processNode() override {
        gameplaySettings = getInputValue<GameplaySettings>("gameplaySettings");
        gameplayData = getInputValue<GameplayData>("gameplayData");
        gameplayState = getInputValue<GameplayState>("gameplayState");

        // Process gameplay
        auto gameplayStatus = updateGameplay(gameplaySettings, gameplayData, gameplayState);

        // Set outputs
        setOutputValue("gameplayStatus", gameplayStatus);
        setOutputValue("gameplayMetrics", computeGameplayMetrics(gameplayStatus));
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayManager() = default;     // Default destructor
};

} // namespace hd
