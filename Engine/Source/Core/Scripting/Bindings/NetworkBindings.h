/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Network system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Network/NetworkSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct NetworkBindingsConfig {
    bool enableReplication = true;
    bool enableCompression = true;
    bool enableEncryption = true;
    bool enableDebugStats = true;
    uint32_t maxConnections = 100;
    uint32_t maxPacketSize = 1024;
    uint32_t sendBufferSize = 32768;  // 32KB
    std::string networkConfigPath = "Config/Network";
};

class NetworkBindings {
public:
    static NetworkBindings& Get();
    
    void Initialize(const NetworkBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Connection management
    ConnectionHandle Connect(const std::string& address, uint16_t port);
    void Disconnect(ConnectionHandle handle);
    bool IsConnected(ConnectionHandle handle) const;
    
    // Message handling
    void SendMessage(ConnectionHandle handle, const NetworkMessage& message);
    void BroadcastMessage(const NetworkMessage& message);
    void RegisterMessageHandler(MessageType type, const MessageHandler& handler);
    void UnregisterMessageHandler(MessageType type);
    
    // Replication
    void RegisterReplicatedObject(const std::string& name, const ReplicatedObject& object);
    void UnregisterReplicatedObject(const std::string& name);
    void UpdateReplicatedObject(const std::string& name, const ReplicatedState& state);
    
    // Network state
    NetworkStats GetConnectionStats(ConnectionHandle handle) const;
    float GetLatency(ConnectionHandle handle) const;
    float GetPacketLoss(ConnectionHandle handle) const;
    
    // Debug utilities
    void EnablePacketLogging(bool enable);
    void DumpNetworkStats(const std::string& path);
    void SimulateLatency(uint32_t milliseconds);
    void SimulatePacketLoss(float percentage);
    
    const NetworkBindingsStats& GetStats() const { return m_Stats; }

private:
    NetworkBindings() = default;
    
    NetworkBindingsConfig m_Config;
    NetworkBindingsStats m_Stats;
    std::unordered_map<ConnectionHandle, ConnectionState> m_Connections;
    std::unordered_map<MessageType, MessageHandler> m_MessageHandlers;
    std::unordered_map<std::string, ReplicatedObjectInstance> m_ReplicatedObjects;
    std::unique_ptr<Network::INetworkProcessor> m_Processor;
    std::unique_ptr<Network::IReplicationManager> m_ReplicationManager;
    std::unique_ptr<Network::IDebugMonitor> m_DebugMonitor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 