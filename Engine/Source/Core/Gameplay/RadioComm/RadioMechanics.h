/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RadioMechanics.h
 * @brief Header file for the RadioMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - RadioMechanics is a class that represents radio mechanics in Hydragon.
 * - It is used to represent any radio mechanics in the game world, such as radio towers, radio dishes, etc.
 * - For now, for the sake of simplicity and to avoid redundancy, we opted to use the existing Device node in place of having a dedicated "Radio" node. So:
 *      
 *      (Audio input chain)
 *      AudioFile --> AudioClip --> AudioProcessor (for radio effects, noise, etc) --> Device --> RadioManager --> other Devices
 * 
 *      (Relationship with Dialogue subsystem)
 *      DialogueBranch or DialogueCheckpoint --> Device --> RadioMechanics --> RadioManager --> other Devices
 * 
 *      (Responsibility chain)
 *      RadioManager --> RadioMechanics --> Device, RadioAntenna, etc
 * 
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other radio mechanics, environment, character, etc.
 */
#pragma once
#include <vector>
#include "Node.h"
#include "Device.h"
#include "RadioChannel.h"

namespace hd {

struct RadioMechanicsInfo : public NodeInfo {
    RadioMechanicsInfo() {
        nodeType = "Gameplay/RadioMechanics";
        
        inputs = {
            "radioMechanicsData",// Radio mechanics data
            "environment",       // Environment data
            "characterData",     // Character data
            "radioMechanicsState"// Radio mechanics state
        };
        
        outputs = {
            "radioMechanicsStatus",// Radio mechanics status
            "radioMechanicsMetrics"// Radio mechanics performance metrics
        };
    }
};

class RadioMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RadioMechanics(const RadioMechanicsInfo& info = RadioMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void addCommand(const std::string& commandName);
    void removeCommand(const std::string& commandName);
    void queueCommand(const std::string& commandName);
    void addResponse(const std::string& responseName);
    void removeResponse(const std::string& responseName);
    processNode() override {}
    update() override {}

    // === Cleanup ===
    cleanup() override {}

private:
    std::vector<std::string> commands;
    std::vector<std::string> responses;
};

}