/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RadioManager.h
 * @brief Header file for the RadioManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - RadioManager is a singleton class that manages the radio in the game.
 * - It is responsible for handling radio-related events, such as broadcasting, receiving, and analyzing.
 * - It uses the FMOD library for radio playback.
 * - For now, for the sake of simplicity, we also use the Radio subsystem to model cable networks, so that the user have less nodes to setup a gameplay network
 * which uses both radio and cable communications. If, for instance, the game asks for some endpoints/connection points (e.g. a modem), he can do so using the Device node.
 * Maybe we change this to be more detailed, with separation of concerns (Radio vs Cable), in the future, based on user feedback, tests.
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
 */
#pragma once
#include "Core/Node.h"
#include "RadioMechanics.h"
#include "Device.h"
#include "RadioChannel.h"

namespace hd {

struct RadioManagerInfo : public NodeInfo {
    RadioManagerInfo() {
        nodeType = "Gameplay/RadioManager";
        
        inputs = {
            "radioData",        // Radio data
            "environment",      // Environment data
            "characterData",    // Character data
            "radioState"        // Radio state
        };
        
        outputs = {
            "radioStatus",      // Radio status
            "radioMetrics"      // Radio performance metrics
        };
    }
};

class RadioManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    RadioManager();
    ~RadioManager();

    // Set default values
    // (...)

    // === Processing ===
    void processNode();

    // === Cleanup ===
    void cleanup();

private:
    Radio* radio;
};

}