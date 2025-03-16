class GameApplication {
private:
    // Multiple node graphs can exist and interconnect
    std::unique_ptr<MultiplayerArenaSetupNode> m_arenaNode;
    std::unique_ptr<NetworkManagerNode> m_networkNode;
    std::unique_ptr<GlobalGameStateNode> m_gameStateNode;
    
    void Initialize() {
        // Create and connect various node graphs
        m_arenaNode = std::make_unique<MultiplayerArenaSetupNode>();
        m_networkNode = std::make_unique<NetworkManagerNode>();
        m_gameStateNode = std::make_unique<GlobalGameStateNode>();
        
        // Nodes/graphs can connect to each other
        m_arenaNode->connect("matchState", m_networkNode.get(), "broadcastState");
        m_arenaNode->connect("playerCount", m_gameStateNode.get(), "activePlayers");
    }
    
    void Update(float dt) {
        // Update all node graphs
        m_arenaNode->Update(dt);
        m_networkNode->Update(dt);
        m_gameStateNode->Update(dt);
    }
};