/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePropagationManager.h
 * @brief This is a singleton class that manages the wave propagation in the game.
 * 
 * ARCHITECTURAL NOTES:
 * - Some brief review of the basic WavePhysics subsystem, regarding its core components:
 * 
 *      Energy (the quantity/data) --> Wave (the carrier pattern: excitation/perturbation/radiation in a field) --> Field (the set/system: energy distribution in space)
 * 
 * - Also, regarding the relatinship between WavePropagationManager, WavePhysics and Wave:
 * 
 *      WavePhysics(the most comprehensive class of the subsystem) --> WavePropagationManager (takes care of wave propagation) --> Wave (the carrier pattern)
 *
 * This modular separation allows for testing, replacing both wave propagation schemes and wave structures.
 * - WavePropagationManager should be the one responsible for "last resort" strategies for wave interference detection (=collisions), like using sphere tracing, to increase the
 * chances of detecting interactions.
 * 
 * @todo After extensive testing, gradually introduce the use of delta encoding to optimize Wave LODs.
 * @todo The handling of interference in the place of using traditional collisions for rigid bodies is one of the most concerning/problematic parts of WavePhysics - do it in
 * a way that is as performant as in traditional RB physics, and also not miss wave interactions, etc.
 * A "last resort" approach is to model the wavefront as a sphere (sphere trace), to considerably raise the chances of detecting interactions. This is not a good solution, 
 * but it's a last resort anyways. In any case, algorithms to detect wave proximity "intra-thresholds" will be necessary.
 */
#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
//#include "glm/gtx/vector_angle.hpp" // For glm::distance() function
//#include "glm/glm.hpp" // For glm::distance() function
#include "Wave.h"

namespace hd {

class WavePropagationManager {
public:
    // === Processing ===
    void createWave(const vec3& position, const vec3& direction, float amplitude, float frequency);
    void detectWaveInteractions();
    void debugVisualizeWaves();                     // TODO: Maybe move debugging functionality to WavePhysics
    void getWaveSpeed(const Wave& wave);            // Returns the speed of the wave based on its frequency
    void calculateWaveDirection(const Wave& wave);  // Normalizes the propagation vector

    void update(float deltaTime);               // Update is automatically called by the Engine every frame. It updates all active waves.
    void updateWavePropagation(float deltaTime);
    void updateWaveEnergy(float deltaTime);     // Updates the energy of all active waves.
    void updateWaveLifetime(float deltaTime);   // Updates the lifetime of all active waves.
    
private:
    // === Allocation, Initialization, Loading ===
    std::vector<Wave> m_waves;
    float m_baseWaveSpeed = 10.0f;
    float m_attenuationFactor = 0.1f;
    float m_defaultLifetime = 5.0f;
    float m_interactionThreshold = 1.0f;
    
    void createInterferenceEffect(const Wave& wave1, const Wave& wave2, float interference);
};

} // namespace hd