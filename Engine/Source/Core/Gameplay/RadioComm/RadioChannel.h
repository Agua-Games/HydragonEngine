/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RadioChannel.h
 * @brief Header file for the RadioChannel class.
 * 
 * ARCHITECTURAL NOTES:
 * - RadioChannel is a class that represents a radio channel in Hydragon.
 * - It is used to represent any radio channel in the game world, such as radio stations, radio frequencies, etc.
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other radio channels, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "RadioMechanics.h"
#include "Device.h"

namespace hd {

struct RadioFrequency {
    float frequency;
    std::string name;
    std::string description;
}

struct RadioChannelInfo : public NodeInfo {
    RadioChannelInfo() {
        nodeType = "Gameplay/RadioChannel";
        
        inputs = {
            "radioChannelType",  // Type of radio channel (radio station, radio frequency, etc.)
            "radioChannelData",  // Radio channel data
            "environment",       // Environment data
            "characterData",     // Character data
            "radioChannelState"  // Radio channel state
        };
        
        outputs = {
            "radioChannelStatus",// Radio channel status
            "radioChannelMetrics"// Radio channel performance metrics
        };
    }
};

class RadioChannel : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RadioChannel(const RadioChannelInfo& info = RadioChannelInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    bool encryption = false;
    RadioFrequency frequency;
    fequency.frequency = 0.0f;
    frequency.name = "";
    frequency.description = "";

    // === Processing ===
    void processNode() override {
 
    }
    void broadcast();
    void receive();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RadioChannel() = default;     // Default destructor
};

} // namespace hd
