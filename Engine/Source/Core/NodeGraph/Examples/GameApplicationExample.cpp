/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

#if 0

class GameApplicationExample_01 {
private:
    // Multiple node graphs can exist and interconnect
    std::unique_ptr<MultiplayerArenaSetup_Example_01> m_arenaNode;
    std::unique_ptr<NetworkManager_Example_01> m_networkNode;
    std::unique_ptr<GlobalGameState_Example_01> m_gameStateNode;
    
    void initialize() {
        // Create and connect various node graphs
        m_arenaNode = std::make_unique<MultiplayerArenaSetup_Example_01>();
        m_networkNode = std::make_unique<NetworkManager_Example_01>();
        m_gameStateNode = std::make_unique<GlobalGameState_Example_01>();
        
        // Nodes/graphs can connect to each other
        m_arenaNode->connect("matchState", m_networkNode.get(), "broadcastState");
        m_arenaNode->connect("playerCount", m_gameStateNode.get(), "activePlayers");
    }
    
    void update(float dt) {
        // Update all node graphs
        m_arenaNode->update(dt);
        m_networkNode->update(dt);
        m_gameStateNode->update(dt);
    }
};

#endif