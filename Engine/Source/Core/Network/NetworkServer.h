/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NetworkServer.h
 * @brief Header file for the NetworkServer class.
 * 
 * ARCHITECTURAL NOTES:
 * - NetworkServer is a class that represents a network server in Hydragon.
 * - It is used to host a network server and send/receive data.
 * - It has built-in support for reliable and unreliable data transmission.
 * - It has built-in support for network replication of objects and variables.
 * - It has built-in support for remote procedure calls (RPCs).
 * 
 */
#pragma once
#include "NetworkManager.h"
#include "Node.h"

namespace hd {

struct NetworkServerInfo : public NodeInfo {
    NetworkServerInfo() {
        NodeType = "Network/NetworkServer";
        inputs = {
            "ServerPort",      // Server port
            "MaxClients"       // Maximum number of clients
        };
        outputs = {
            "ConnectionStatus",  // Connection status
            "NetworkMetrics"     // Network performance metrics
        };
    }
};

class NetworkServer : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit NetworkServer(const NetworkServerInfo& info = NetworkServerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~NetworkServer() = default;
};

} // namespace hd