/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NetworkClient.h
 * @brief Header file for the NetworkClient class.
 * 
 * ARCHITECTURAL NOTES:
 * - NetworkClient is a class that represents a network client in Hydragon.
 * - It is used to connect to a network server and send/receive data.
 * - It has built-in support for reliable and unreliable data transmission.
 * - It has built-in support for network replication of objects and variables.
 * - It has built-in support for remote procedure calls (RPCs).
 * 
 */
#pragma once
#include "NetworkManager.h"
#include "Node.h"

namespace hd {

struct NetworkClientInfo : public NodeInfo {
    NetworkClientInfo() {
        NodeType = "Network/NetworkClient";
        inputs = {
            "ServerAddress",   // Server address
            "ServerPort",      // Server port
            "ClientID"         // Client ID
        };
        outputs = {
            "ConnectionStatus",  // Connection status
            "NetworkMetrics"     // Network performance metrics
        };
    }
};

class NetworkClient : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit NetworkClient(const NetworkClientInfo& info = NetworkClientInfo())
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
    ~NetworkClient() = default;
};

} // namespace hd