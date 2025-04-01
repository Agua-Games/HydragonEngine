/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NetworkManager.h
 * @brief Header file for the NetworkManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - NetworkManager is a class that manages the network connections and data transmission.
 * - It is used to create and manage network clients and servers.
 * - It has built-in support for reliable and unreliable data transmission.
 * - It has built-in support for network replication of objects and variables.
 * - It has built-in support for remote procedure calls (RPCs).
 * 
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct NetworkManagerInfo : public NodeInfo {
    NetworkManagerInfo() {
        NodeType = "Network/NetworkManager";
        inputs = {
            "NetworkSettings" // Network settings
        };
        outputs = {
            "NetworkStatus" // Network status
        };
    }
};

class NetworkManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit NetworkManager(const NetworkManagerInfo& info = NetworkManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    int tickRate = 30;
    bool interpolation = false;
    bool replication = false;
    bool prediction = false;
    int lookAhead = 0;
    float maxExtrapolation = 0.0f;
    bool rollback = false;
    bool rpc = false;
    bool lagCompensation = false;
    int maxClients = 10;                // TODO: Decide if calling it maxPlayers instead
    DataTable networkSettings;

    // === Processing ===
    void processNode() override {
 
    }
    void setupMatchFlow();
    void setupNetworkPrediction();
    void setPrediction();
    void enablePrediction();
    void setInterpolation();
    void processNetwork();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~NetworkManager() = default;
};

} // namespace hd