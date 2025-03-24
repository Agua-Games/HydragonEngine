/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioListener.h
 * @brief Header file for the AudioListener class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioListener is a node class that represents an audio listener.
 * - It provides methods to set the position, orientation, and velocity of the listener.
 * - It also provides methods to get the current position, orientation, and velocity of the listener.
 */
#pragma once
#include <fmod.hpp>
#include <string>
#include "Node.h"

namespace hd {

struct AudioListenerInfo : public NodeInfo {
    AudioListenerInfo() {
        NodeType = "Audio/AudioListener";
        inputs = {
            "Position",      // Listener position
            "Orientation",   // Listener orientation
            "Velocity"       // Listener velocity
        };
        outputs = {
            "ListenerData"   // Listener data
        };
    }
};

class AudioListener : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioListener(const AudioListenerInfo& info = AudioListenerInfo())
        : Node(info), AudioListenerInfo(info) {}
    
    void initialize() override {}
    void load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioListener() = default;     // Default destructor
};

} // namespace hd

