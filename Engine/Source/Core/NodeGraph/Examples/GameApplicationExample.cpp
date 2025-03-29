/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Game application setup with multiple node graphs
 *  - Interconnection between node graphs
 */
#if 0
#pragma once
#include "MultiplayerArenaSetup_Example_01.h"
#include "NetworkManager_Example_01.h"
#include "GlobalGameState_Example_01.h"

using namespace hd;

class GameApplicationExample_01 {
private:
    // Multiple node graphs can exist and interconnect
    std::unique_ptr<MultiplayerArenaSetup_Example_01> m_arena;
    std::unique_ptr<NetworkManager_Example_01> m_network;
    std::unique_ptr<GlobalGameState_Example_01> m_gameState;
    
    void initialize() {
        // Create and connect various node graphs
        m_arena = std::make_unique<MultiplayerArenaSetup_Example_01>();
        m_network = std::make_unique<NetworkManager_Example_01>();
        m_gameState = std::make_unique<GlobalGameState_Example_01>();
        
        // Nodes/graphs can connect to each other
        m_arena->connect("matchState", m_network.get(), "broadcastState");
        m_arena->connect("playerCount", m_gameState.get(), "activePlayers");
    }
    
    void update(float dt) {
        // Update all node graphs
        m_arena->update(dt);
        m_network->update(dt);
        m_gameState->update(dt);
    }
};

#endif