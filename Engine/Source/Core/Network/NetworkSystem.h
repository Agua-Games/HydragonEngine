/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core networking and multiplayer system
 */

#pragma once
#include "NetworkTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <functional>

namespace Hydragon::Network {

class NetworkSystem {
public:
    struct NetworkConfig {
        uint32_t maxConnections = 32;
        uint32_t maxChannels = 8;
        bool enableCompression = true;
        bool enableEncryption = true;
        bool enableReliableUDP = true;
        size_t sendBufferSize = 1024 * 1024;  // 1MB
        size_t receiveBufferSize = 1024 * 1024;  // 1MB
        float timeoutSeconds = 30.0f;
    };

    static NetworkSystem& Get();
    
    void Initialize(const NetworkConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ConnectionHandle Connect(const ConnectionDesc& desc);
    void Disconnect(ConnectionHandle handle);
    
    void Send(ConnectionHandle conn, const NetworkMessage& msg, const SendConfig& config = {});
    void Broadcast(const NetworkMessage& msg, const SendConfig& config = {});
    
    void AddMessageHandler(MessageType type, const MessageHandler& handler);
    void RemoveMessageHandler(MessageType type);
    
    void EnableReplication(EntityHandle entity);
    void DisableReplication(EntityHandle entity);
    
    void SetReplicationFilter(EntityHandle entity, const ReplicationFilter& filter);
    
    const NetworkStats& GetStats() const { return m_Stats; }

private:
    NetworkSystem() = default;
    
    NetworkConfig m_Config;
    NetworkStats m_Stats;
    std::unique_ptr<INetworkProvider> m_Provider;
    std::vector<Connection> m_Connections;
    std::vector<ReplicatedEntity> m_ReplicatedEntities;
    bool m_Initialized = false;
};

} // namespace Hydragon::Network 