/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePropagationManager.cpp
 * @brief This is a singleton class that manages the wave propagation in the game.
 */
#include "WavePropagationManager.h"

namespace hd {

// === Processing ===
void WavePropagationManager::createWave(const vec3& position, const vec3& direction, 
    float amplitude, float frequency) {
    Wave wave;
    wave.position = position;
    wave.direction = glm::normalize(direction);
    wave.amplitude = amplitude;
    wave.frequency = frequency;
    wave.phase = 0.0f;
    wave.velocity = m_baseWaveSpeed;
    wave.energy = 0.5f * amplitude * amplitude;
    wave.lifetime = m_defaultLifetime;

    m_waves.push_back(wave);
}

void WavePropagationManager::detectWaveInteractions() {
    // Simple O(n²) interaction detection for prototype
    for (size_t i = 0; i < m_waves.size(); i++) {
        for (size_t j = i + 1; j < m_waves.size(); j++) {
            // Check if waves are close enough to interact
            float distance = glm::distance(m_waves[i].position, m_waves[j].position);
            
            if (distance < m_interactionThreshold) {
                // Calculate interference
                float phaseDifference = m_waves[i].phase - m_waves[j].phase;
                float interference = std::cos(phaseDifference);
                
                // Create interference effect
                if (std::abs(interference) > 0.7f) {
                    createInterferenceEffect(m_waves[i], m_waves[j], interference);
                }
            }
        }
    }
}

void WavePropagationManager::update(float deltaTime) {
    // Update all active waves
    for (auto& wave : m_waves) {
        // Update position
        wave.position += wave.direction * wave.velocity * deltaTime;
        
        // Update phase
        wave.phase += wave.frequency * deltaTime;
        
        // Attenuate amplitude with distance
        wave.amplitude *= std::exp(-m_attenuationFactor * deltaTime);
        
        // Update energy
        wave.energy = 0.5f * wave.amplitude * wave.amplitude;
        
        // Update lifetime
        wave.lifetime -= deltaTime;
    }
    
    // Remove expired waves
    m_waves.erase(
        std::remove_if(m_waves.begin(), m_waves.end(),
                      [](const Wave& w) { return w.lifetime <= 0.0f || w.energy < 0.001f; }),
        m_waves.end());
    
    // Check for wave interactions
    detectWaveInteractions();
}

void WavePropagationManager::createInterferenceEffect(const Wave& wave1, const Wave& wave2, float interference) {
    // Create visual effect at midpoint
    vec3 midpoint = (wave1.position + wave2.position) * 0.5f;
    
    // Constructive interference
    if (interference > 0) {
        // Create enhanced wave
        float combinedAmplitude = wave1.amplitude + wave2.amplitude;
        createWave(midpoint, (wave1.direction + wave2.direction) * 0.5f,
                    combinedAmplitude, (wave1.frequency + wave2.frequency) * 0.5f);
    }
    // Destructive interference
    else {
        // Create reduced wave
        float differenceAmplitude = std::abs(wave1.amplitude - wave2.amplitude);
        if (differenceAmplitude > 0.1f) {
            createWave(midpoint, (wave1.direction + wave2.direction) * 0.5f,
                        differenceAmplitude, (wave1.frequency + wave2.frequency) * 0.5f);
        }
    }
}

} // namespace hd