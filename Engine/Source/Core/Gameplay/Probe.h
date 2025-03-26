/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Probe.h
 * @brief Header file for the Probe class.
 * 
 * ARCHITECTURAL NOTES:
 * - Probe is a class that represents a probe in Hydragon.
 * - It is used to represent any probe in the game world, such as space probes, buoys, sensors, cameras, etc.
 * - It supports interactive features, such as scanning, monitoring, and analyzing. Also supports two-way messaging with other probes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct ProbeInfo : public NodeInfo {
    ProbeInfo() {
        nodeType = "Gameplay/Probe";
        
        inputs = {
            "model",        // Model of the probe
            "position",     // Position of the probe
            "rotation",     // Rotation of the probe
            "scale",        // Scale of the probe
            "material",     // Material of the probe
            "animation",    // Animation of the probe
            "physics",      // Physics of the probe
            "collision",    // Collision of the probe
            "script"        // Script of the probe
        };
        
        outputs = {
            "probeStatus",  // Probe status
            "probeMetrics"  // Probe performance metrics
        };
    }
};

class Probe : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Probe(const ProbeInfo& info = ProbeInfo())
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
    ~Probe() = default;     // Default destructor
};

} // namespace hd
