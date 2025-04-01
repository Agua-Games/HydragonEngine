/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameplayData.h
 * @brief Header file for the GameplayData class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameplayData is a class that represents gameplay data in Hydragon.
 * - It is used to represent any gameplay data in the game world, such as player stats, enemy health, etc.
 * - It supports interactive features, such as modifying, updating, and accessing. Also supports two-way messaging with other gameplay data, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "GameplayData.h"
#include "DataTable.h"

namespace hd {

struct GameplayDataInfo : public NodeInfo {
    GameplayDataInfo() {
        nodeType = "Gameplay/GameplayData";
        inputs = {
            "data",          // Gameplay data
            "environment",   // Environment data
            "characterData"  // Character data
        };
        outputs = {
            "dataStatus",    // Gameplay data status
            "dataMetrics"    // Gameplay data performance metrics
        };
    }
};

class GameplayData : public Node {
public:
    // === Structure Definitions ===
    enum class GameplayMode {
        CaptureTheFlag,
        KingOfTheHill,
        TeamDeathmatch,
        FreeForAll,
        CaptureThePoint,
        Domination,
        Elimination,
        LastManStanding,
        Objective,
        Custom
    };

    struct GameplayRules {
        // Data for gameplay rules
    };

    struct CaptureTheFlag {
        float captureTime;   // Time to capture the flag
        float scoreLimit;    // Score limit to win the game
        float timeLimit;     // Time limit to win the game
        bool overtime;        // Enable overtime
        bool friendlyFire;    // Enable friendly fire
    };

    struct KingOfTheHill {
        float hillRadius;    // Radius of the hill
        float moveInterval;  // Interval to move the hill
        bool overtime;        // Enable overtime
        bool friendlyFire;    // Enable friendly fire
    };

    struct TeamDeathmatch {
        float scoreLimit;    // Score limit to win the game
        float timeLimit;     // Time limit to win the game
        bool overtime;        // Enable overtime
        bool friendlyFire;    // Enable friendly fire
    };

    struct FreeForAll {
        float scoreLimit;    // Score limit to win the game
        float timeLimit;     // Time limit to win the game
        bool overtime;        // Enable overtime
        bool friendlyFire;    // Enable friendly fire
    };

    struct CaptureThePoint {
        float captureTime;   // Time to capture the flag
        float scoreLimit;    // Score limit to win the game
        float timeLimit;     // Time limit to win the game
        bool overtime;        // Enable overtime
    };

    struct Domination {
        float dominationTime;   // Time to dominate the point
        float scoreLimit;       // Score limit to win the game
        float timeLimit;        // Time limit to win the game
        bool overtime;          // Enable overtime
    };

    struct Elimination {
        // Data for Elimination mode
    };

    struct LastManStanding {
    };

    struct Objective {
        // Data for Objective mode
    };

    struct Settings {
        // Settings for gameplay
        bool enableDestructibles;
    };

    struct GlobalFlags {
        // Global flags for gameplay
    };

    struct LocalFlags {
        // Local flags for gameplay. Uses value pairs to set the locality/scope
    };

    struct Stats {
        // Stats for gameplay
    };

    struct State {
        enum class MatchState {
            Warmup,
            Playing,
            Overtime,
            EndGame
        } MatchState matchState;

        enum class RoundState {
            RoundStart,
            RoundInProgress,
            RoundEnd
        } RoundState roundState;

        void setMatchState(MatchState state) {
            matchState = state;
        }
    };

    // === Allocation, Initialization, Loading === 
    explicit GameplayData(const GameplayDataInfo& info = GameplayDataInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    int maxPlayers = 0;
    int maxSpawns = 0;
    float respawnTime = 0.0f;
    float timeLimit = 0.0f;
    int maxSpectators = 0;
    bool overtime = false;
    bool killCam = false;
    bool classLimits = false;

    // === Processing ===
    void processNode() override {
 
    }
    void update();
    void checkGlobalFlags();    // Check global flags for gameplay. Uses value pairs to set the locality/scope.
    void setGlobalFlags();      // Set global flags for gameplay. Uses value pairs to set the locality/scope.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GameplayData() = default;     // Default destructor
};

} // namespace hd
