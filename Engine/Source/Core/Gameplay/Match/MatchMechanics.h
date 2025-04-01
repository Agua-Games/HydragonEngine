/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MatchMechanics.h
 * @brief Header file for the MatchMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - MatchMechanics is a class that represents match mechanics in Hydragon.
 * - It is used to represent any match mechanics in the game world, such as match start, match end, etc.
 * - It supports interactive features, such as triggering events and responding to events. Also supports two-way messaging with other match mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct MatchMechanicsInfo : public NodeInfo {
    MatchMechanicsInfo() {
        nodeType = "Gameplay/MatchMechanics";
        
        inputs = {
            "matchMechanicsData",// Match mechanics data
            "environment",       // Environment data
            "characterData",     // Character data
            "matchMechanicsState"// Match mechanics state
        };
        
        outputs = {
            "matchMechanicsStatus",// Match mechanics status
            "matchMechanicsMetrics"// Match mechanics performance metrics
        };
    }
};

class MatchMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit MatchMechanics(const MatchMechanicsInfo& info = MatchMechanicsInfo())
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
    ~MatchMechanics() = default;     // Default destructor
};

} // namespace hd

