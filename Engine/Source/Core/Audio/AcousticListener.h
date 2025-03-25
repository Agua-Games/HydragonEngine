/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticListener.h
 * @brief Header file for the AcousticListener class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticListener is a node class that represents an audio listener.
 * - It provides methods to set the position, orientation, and velocity of the listener.
 * - It also provides methods to get the current position, orientation, and velocity of the listener.
 */
#pragma once
#include <fmod.hpp>
#include <string>
#include "Node.h"
#include "AcousticProcessor.h"

namespace hd {

struct AcousticListenerInfo : public NodeInfo {
    AcousticListenerInfo() {
        NodeType = "Audio/AcousticListener";
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

class AcousticListener : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AcousticListener(const AcousticListenerInfo& info = AcousticListenerInfo())
        : Node(info), AcousticListenerInfo(info) {}
    
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
    ~AcousticListener() = default;     // Default destructor

private:
    // === Processing ===
    void processWavefront(const AcousticWave& wave) {
        // Calculate wave interaction at listener position
        auto interaction = computeWaveInteraction(
            wave.getSignature(),
            getPosition(),
            wave.getMediumProperties()
        );

        // Apply acoustic effects based on medium properties
        auto modulated = m_acousticProcessor.process(
            interaction,
            getCurrentMedium()
        );

        // Play matched audio with physics-based modulation
        for (const auto& asset : wave.getMatchedAssets()) {
            playModulatedAudio(asset, modulated);
        }
    }
};

} // namespace hd

