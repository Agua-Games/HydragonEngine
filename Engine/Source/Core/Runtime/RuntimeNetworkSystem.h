/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime network and multiplayer management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeNetworkConfig {
    bool enableReliableUDP = true;
    bool enableCompression = true;
    bool enableEncryption = true;
    bool enableReplication = true;
    uint32_t maxConnections = 64;
    uint32_t maxPacketSize = 1400;
    uint32_t replicationRate = 20;  // updates per second
    float connectionTimeout = 5.0f;
    std::string networkProtocol = "UDP";
};

class RuntimeNetworkSystem {
public:
    static RuntimeNetworkSystem& Get();
    
    void Initialize(const RuntimeNetworkConfig& config = {});
    void Shutdown();
    
    void Update();
    
    NetworkSessionHandle CreateSession(const SessionDesc& desc);
    void DestroySession(NetworkSessionHandle handle);
    void JoinSession(const std::string& address, const JoinParams& params = {});
    void LeaveSession();
    
    ConnectionHandle ConnectPeer(const PeerDesc& desc);
    void DisconnectPeer(ConnectionHandle handle);
    
    void SendData(ConnectionHandle handle, const NetworkPacket& packet, SendPriority priority = SendPriority::Normal);
    void BroadcastData(const NetworkPacket& packet, SendPriority priority = SendPriority::Normal);
    
    void EnableReplication(EntityHandle entity, const ReplicationConfig& config = {});
    void DisableReplication(EntityHandle entity);
    void SetReplicationAuthority(EntityHandle entity, ConnectionHandle authority);
    
    void RegisterPacketHandler(PacketType type, const PacketHandler& handler);
    void UnregisterPacketHandler(PacketType type);
    
    void RegisterReplicationFilter(const std::string& name, std::unique_ptr<IReplicationFilter> filter);
    void UnregisterReplicationFilter(const std::string& name);
    
    NetworkState GetNetworkState() const;
    SessionState GetSessionState() const;
    ConnectionState GetPeerState(ConnectionHandle handle) const;
    const RuntimeNetworkStats& GetStats() const { return m_Stats; }

private:
    RuntimeNetworkSystem() = default;
    
    RuntimeNetworkConfig m_Config;
    RuntimeNetworkStats m_Stats;
    std::unordered_map<NetworkSessionHandle, SessionInstance> m_Sessions;
    std::unordered_map<ConnectionHandle, PeerInstance> m_Peers;
    std::unordered_map<EntityHandle, ReplicatedEntity> m_ReplicatedEntities;
    std::unordered_map<PacketType, PacketHandler> m_PacketHandlers;
    std::unordered_map<std::string, std::unique_ptr<IReplicationFilter>> m_ReplicationFilters;
    std::unique_ptr<INetworkTransport> m_Transport;
    std::unique_ptr<IReplicationManager> m_ReplicationManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 