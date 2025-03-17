/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

#include "Core/Engine.h"
#include "Core/Network/NetworkManager.h"
#include "Core/Physics/PhysicsWorld.h"
#include "Core/Gameplay/TeamManager.h"
#include "Core/NodeGraph/Node.h"

namespace hd {

class MultiplayerArenaSetup_Example_01 : public Node {
public:
    void setup() {                              // Going to be replace with Initialize()
        auto& engine = Engine::getInstance();
        auto& graph = engine.getNodeGraph();

        // Network setup
        NetworkManager network;
        network.setTickRate(128)
               .setInterpolation(true)
               .setLagCompensation(true)
               .setMaxPlayers(32);

        // Arena scene
        ArenaScene arena("BattleArena");
        arena.loadGeometry("Arenas/Cyberpunk/Main.usd")
             .setLightingSetup("Dynamic")
             .enableDestructibles(true)
             .setRespawnPoints("Arenas/Cyberpunk/Spawns.json");

        // Team management
        TeamManager teams;
        teams.setTeamCount(2)
             .setTeamSize(16)
             .setAutoBalance(true)
             .enableFriendlyFire(false);

        // Gameplay systems
        GameplayManager gameplay;
        gameplay.setGameMode("TeamDeathmatch")
                .setScoreLimit(100)
                .setTimeLimit(600) // seconds
                .enableOvertime(true);

        // Player management
        PlayerManager players;
        players.setLoadouts("Config/Loadouts.json")
               .enableClassLimits(true)
               .setRespawnTime(5.0f)
               .enableKillcam(true);

        // Physics setup
        PhysicsWorld physics;
        physics.setSimulationRate(240)
               .setGravity(Vector3(0, -9.81f, 0))
               .enableContinuousDetection(true)
               .setSubsteps(2);

        // Powerup system
        PowerupSystem powerups;
        powerups.setSpawnInterval(30.0f)
                .addType("HealthPack", 0.4f)
                .addType("Shield", 0.3f)
                .addType("Damage", 0.3f);

        // Match state management
        MatchStateManager matchState;
        matchState.addState("Warmup", 30)
                 .addState("Playing")
                 .addState("Overtime")
                 .addState("EndGame", 10);

        // Spectator system
        SpectatorSystem spectators;
        spectators.setMaxSpectators(32)
                 .enableFreeCam(true)
                 .enablePlayerPOV(true)
                 .setReplayBuffer(30.0f); // seconds

        // Connect systems
        graph.connect(network, "PlayerJoined", teams, "AssignTeam");
        graph.connect(teams, "TeamBalance", gameplay, "TeamData");
        graph.connect(gameplay, "MatchState", matchState, "StateUpdate");
        graph.connect(physics, "Collision", gameplay, "DamageEvent");

        // Player feedback
        graph.connect(players, "PlayerDeath", spectators, "StartSpectating");
        graph.connect(powerups, "PowerupCollected", players, "ApplyPowerup");

        // Setup match flow
        setupMatchFlow(matchState, gameplay);
        setupNetworkPrediction(network, physics);
    }

private:
    void setupMatchFlow(MatchStateManager& matchState, GameplayManager& gameplay) {
        MatchFlow flow;
        flow.addTransition("Warmup", "Playing", "AllPlayersReady")
            .addTransition("Playing", "Overtime", "ScoreTied")
            .addTransition("Playing", "EndGame", "ScoreReached")
            .addTransition("Overtime", "EndGame", "OvertimeScore");

        matchState.setFlowLogic(flow);
        gameplay.setMatchFlow(flow);
    }

    void setupNetworkPrediction(NetworkManager& network, PhysicsWorld& physics) {
        PredictionSystem prediction;
        prediction.setLookAhead(2) // ticks
                 .setMaxExtrapolation(0.2f) // seconds
                 .enableRollback(true);

        network.setPrediction(prediction);
        physics.enablePrediction(prediction);
    }
};

} // namespace hd