class MultiplayerArenaSetupNode : public SceneNode {
public:
    MultiplayerArenaSetupNode() {
        // Internal node graph setup
        m_graph.add<SpawnPointManagerNode>("spawnPoints")
            .maxPlayers(32, runtime)
            .connect("playerSpawned", "gameMode.onPlayerJoined")
        .add<GameModeNode>("gameMode")
            .mode(GameMode::TeamDeathmatch)
            .connect("scoreUpdated", "ui.score")
        .add<WeaponSpawnerNode>("weapons")
            .spawnInterval(30.0f, runtime)
            .connect("weaponPicked", "gameMode.onItemPickup");
            
        // Expose external ports for connecting this node/graph to others
        ExposePort("matchState", PortType::Output);
        ExposePort("playerCount", PortType::Output);
    }
};