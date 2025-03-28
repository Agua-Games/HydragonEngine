/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioManager.h
 * @brief Header file for the AudioManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioManager is a singleton class that manages the audio playback.
 * - It is responsible for playing, pausing, stopping, and managing the audio system.
 * - It uses the FMOD library for audio playback.
 * - To understand the Audio system, the function of each node:
 * 
 *      AudioFile --> AudioClip (for settings, some FX) --> AudioProcessor (for FX, modulation. Or use EQ, etc.) --> Montage --> AudioManager
 * 
  * @todo to other managers, where applicable: Introduce functionality to them (not here) to disable their processing, so that they can be disabled selectively, when
  * in the editor.
 */
#pragma once
#include <fmod.hpp>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "PhysicsTypes.h"
#include "AcousticProcessor.h"
/* #include "AudioPool.h"           // Nice suggestions for interfaces
#include "WavePool.h"
#include "AudioAsset.h"
#include "AudioConfig.h"
#include "AudioEvent.h"
#include "AudioTypes.h" */

namespace hd {

struct AudioManagerInfo : public NodeInfo {
    AudioManagerInfo() {
        nodeType = "Audio/AudioManager";
        inputs = {
            "audioClips",    // Array of audio clips
            "audioSettings"  // Audio settings
        };
        outputs = {
            "audioStatus",   // Audio playback status
            "audioMetrics"   // Audio performance metrics
        };
    }
};

class AudioManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    AudioManager(const AudioManagerInfo& info = AudioManagerInfo());
    static AudioManager* getInstance();    // Get the singleton instance of the AudioManager.

    // These are called once, when the engine is initialized.
    void initialize() override;
    void load() override;

    // Set default values
    std::vector<AudioClip*> audioClips;
    AudioSettings audioSettings;
    bool m_active = true;    // Active by default, can be disabled

    void processNode() override {
        if (!m_active) return;
        
        // Normal processing when active
        audioClips = getInputValue<std::vector<AudioClip*>>("audioClips");
        audioSettings = getInputValue<AudioSettings>("audioSettings");

        // Process audio
        auto audioStatus = updateAudio(audioClips, audioSettings);

        // Set outputs
        setOutputValue("audioStatus", audioStatus);
        setOutputValue("audioMetrics", computeAudioMetrics(audioStatus));
    }
    
    void play(const std::string& audioClipName, const vec3& position);
    void play(const std::string& audioClipName);
    void updateResolution(const vec3& listenerPos) {
        // Adjust wave sampling based on distance and importance
        for (auto& wave : m_activeWaves) {
            float distance = (wave.position - listenerPos).length();
            float importance = wave.getEnergy() / (distance * distance);
            
            wave.setSamplingResolution(
                computeAdaptiveResolution(importance)
            );
        }
    }

    void computeWaveInteraction(const AcousticWave& wave, const vec3& position, const MediumProperties& medium);

    void handleEnergyTransfer(const EnergyTransferEvent& event) {
        if (event.getKineticEnergy() > m_config.acousticThreshold) {
            spawnAcousticWave(event);
        }
    }
    void handleCollision(const CollisionEvent& event) {
        if (event.getImpactEnergy() > m_config.acousticThreshold) {
            spawnAcousticWave(event);
        }
    }
    void spawnAcousticWave(const EnergyTransferEvent& event) {
        auto wave = AcousticWave(event.getPosition(), event.getKineticEnergy());
        m_wavePool.spawn(wave);
    }
    void processAcousticWave(const AcousticWave& wave) {
        if (wave.isWithinRange(getPosition(), m_config.acousticRange)) {
            processWavefront(wave);
        }
    }

    void processWavefront(const AcousticWave& wave) {
        auto interaction = computeWaveInteraction(
            wave.getSignature(),
            getPosition(),
            wave.getMediumProperties()
        );

        auto modulated = m_acousticProcessor.process(
            interaction,
            getCurrentMedium()
        );

        for (const auto& asset : wave.getMatchedAssets()) {
            playModulatedAudio(asset, modulated);
        }
    }

    void playModulatedAudio(const AudioAsset& asset, const AcousticModulation& modulation) {
        auto audio = m_audioPool.acquire(asset);
        audio.applyModulation(modulation);
        audio.play();
    }

    void () override;     // This is called every frame. It updates the audio system.
    void update();

    // === Cleanup ===
    // These are called once, when the engine is cleaned up.
    void unload();
    void cleanup();
    ~AudioManager();
};

} // namespace hd
