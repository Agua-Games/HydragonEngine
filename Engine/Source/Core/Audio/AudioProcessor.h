/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioProcessor.h
 * @brief Header file for the AudioProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioProcessor is a class that represents an audio processor in Hydragon.
 * - It is used to process audio signals and apply various audio effects, such as reverb, compression, equalization, etc.
 * - The processing path is automatically optimized: params left at 0.0 will be skipped in the processing chain.
 * - It encapsulates different audio processing algorithms and provides a unified interface for applying them.
 * - To understand the Audio system, the function of each node:
 * 
 *      AudioFile --> AudioClip (for settings, some FX) --> AudioProcessor (for FX, modulation. Or use EQ, etc.) --> Montage --> AudioManager
 * 
 * - It uses the FMOD library for audio processing.
 * 
 */
#pragma once
//#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "Reverb.h"
#include "Compressor.h"
#include "EQ.h"
#include "AudioFile.h"
#include "PhysicsTypes.h"
#include "AcousticProcessor.h"
/* #include "AudioPool.h"           // Nice suggestions for interfaces
#include "WavePool.h"
#include "AudioAsset.h"
#include "AudioConfig.h"
#include "AudioEvent.h"
#include "AudioTypes.h" */

namespace hd {

struct AudioProcessorInfo : public NodeInfo {
    AudioProcessorInfo() {
        NodeType = "Audio/AudioProcessor";
        inputs = {
            "AudioSignal",   // Audio signal to process
            "Reverb",
            "Compression",
            "Equalization",
            "SideChain",
            "AudioParams"
        };
        outputs = {
            "ProcessedSignal",
            "AudioMetrics"
        };
    }
};

class AudioProcessor : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AudioProcessor(const AudioProcessorInfo& info = AudioProcessorInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    Reverb reverb;
    Compressor compressor;
    EQ eq;

    // === Processing ===
    using AudioSignal = std::vector<float>;
    using AudioParams = std::unordered_map<std::string, float>;

    void applyReverb(const Reverb& reverb);
    void applyCompression(const Compressor& compressor);
    void applyEqualization(const EQ& eq);
    AudioSignal process(const AudioSignal& signal);
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AudioProcessor() = default;     // Default destructor
};

} // namespace hd
