/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Network system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Network {

using ConnectionHandle = uint32_t;
using EntityHandle = uint32_t;
using MessageType = uint32_t;

enum class ConnectionType {
    Client,
    Server,
    P2P
};

enum class ChannelType {
    Reliable,
    Unreliable,
    ReliableOrdered,
    UnreliableOrdered
};

struct ConnectionDesc {
    std::string address;
    uint16_t port;
    ConnectionType type;
    std::string protocol = "UDP";
    bool encrypted = true;
    float timeout = 30.0f;
};

struct NetworkMessage {
    MessageType type;
    std::vector<uint8_t> data;
    ChannelType channel;
    uint32_t sequence;
};

struct SendConfig {
    ChannelType channel = ChannelType::Reliable;
    bool compress = true;
    bool encrypt = true;
    float timeout = 5.0f;
    uint32_t retries = 3;
};

struct ReplicationFilter {
    float updateRate = 0.1f;  // 10Hz
    float relevanceDistance = 100.0f;
    bool fullState = false;
    std::vector<std::string> includedProperties;
};

using MessageHandler = std::function<void(ConnectionHandle, const NetworkMessage&)>;

struct NetworkStats {
    uint32_t activeConnections;
    uint32_t messagesSent;
    uint32_t messagesReceived;
    uint32_t bytesIn;
    uint32_t bytesOut;
    float averageLatencyMs;
    float packetLoss;
    size_t memoryUsed;
};

} // namespace Hydragon::Network 