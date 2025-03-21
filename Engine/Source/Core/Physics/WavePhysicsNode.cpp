/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePhysicsNode.cpp
 * 
 */
#include <glm/gtx/vector_angle.hpp>
#include "WavePhysicsNode.h"
#include "WaveletTransform.h"           // TODO: a dedicated class for the wavelet transform algorithm

namespace hd {

float EnergyDimension::ExcitationField::sampleField(const vec3& position, float energyLevel) const {
    // Warp the sampling based on energy level
    vec3 warpedPos = position + 
        vec3(simplex3D(position * energyLevel * m_warpFactor));
    
    // Multi-octave sampling for different energy scales
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = m_baseFrequency;
    
    for(int i = 0; i < m_octaves; ++i) {
        result += amplitude * simplex4D(vec4(
            warpedPos * frequency, 
            energyLevel * m_energyScale
        ));
        
        frequency *= 2.0f;
        amplitude *= 0.5f;
    }
    
    return result;
}

void EnergyDimension::ModulatedWave::interact(const ExcitationField& field, const vec3& position) {
    float fieldValue = field.sampleField(position, 
                                        energySpectrum.toScalar());
    
    // Modulate wave properties based on field interaction
    EnergySpectrum modulated = energySpectrum.modulate(fieldValue);
    
    // Update wave characteristics
    baseProperties.frequency *= 
        1.0f + fieldValue * m_config.frequencyModulation;
    
    // Quantum effects at high energy levels
    if (modulated.toScalar() > FrequencyBand::kXRay) {
        triggerQuantumEffects(modulated);
    }
}

void EnergyDimension::propagateModulatedWave(const vec3& origin, 
                                            const vec3& direction, 
                                            const EnergySpectrum& initialEnergy) {
    ModulatedWave wave;
    wave.energySpectrum = initialEnergy;
    wave.baseProperties = createBaseWaveProperties(initialEnergy);

    // Sample energy field at propagation points
    vec3 samplePoint = origin;
    float stepSize = m_config.propagationStepSize;

    while (wave.energySpectrum.toScalar() > m_config.minEnergyThreshold) {
        // Interact with energy dimension
        wave.interact(m_excitationField, samplePoint);

        // Update propagation
        samplePoint += direction * stepSize;

        // Generate field effects based on modulated energy
        if (shouldGenerateEffects(wave)) {
            generateFieldEffects(samplePoint, wave);
        }
    }
}

void EnergyDimension::triggerQuantumEffects(const EnergySpectrum& energy) {
    // Implement quantum effects based on high energy levels
}

void WaveTrackingSystem::updateWavePropagation(float deltaTime) {
    for (auto& particle : m_particleSystem.getActiveParticles()) {
        // Update wave properties
        auto& wave = m_energyField.getWaveAt(particle.currentPos);
        wave.amplitude *= calculateEnergyDissipation(particle, deltaTime);
        
        // Update propagation
        vec3 newDirection = calculateWaveDirection(particle, m_energyField);
        particle.velocity = newDirection * getWaveSpeed(wave);
        
        // Generate field effects
        if (particle.energy > m_config.minEnergyThreshold) {
            m_energyField.propagateWave(
                particle.currentPos,
                particle.velocity,
                particle.energy
            );
        }
    }
}

vec3 WaveTrackingSystem::calculateWaveDirection(const ShortLivedParticle& wavefront, 
    const EnergyField& field) {
    // Consider field gradients and medium properties
    vec3 baseDirection = wavefront.velocity;
    vec3 fieldInfluence = field.getGradientAt(wavefront.currentPos);

    return glm::normalize(baseDirection + fieldInfluence * m_config.fieldInfluence);
}

static ForceFieldConfig::RangeProfile ForceFieldConfig::RangeProfile::createGravityLike() {
    return { 100.0f, 2.0f, 0.1f }; // Long range, inverse square
}

static ForceFieldConfig::RangeProfile ForceFieldConfig::RangeProfile::createContactForce() {
    return { 1.0f, 4.0f, 10.0f };  // Short range, strong
}

void ForceFieldProcessor::processFieldBlock(GridBlock& block) {
    // Process 8x8x8 blocks using vectorized operations
    #pragma omp parallel for collapse(3)
    for (uint32_t x = 0; x < BLOCK_SIZE; x++) {
        for (uint32_t y = 0; y < BLOCK_SIZE; y++) {
            for (uint32_t z = 0; z < BLOCK_SIZE; z++) {
                // SIMD-friendly field calculations
            }
        }
    }
}

void WaveBatchProcessor::processBatch(WaveBatch& batch) {
    // Process 8 waves simultaneously using SIMD
    #pragma omp simd
    for (uint32_t i = 0; i < batch.activeCount; i++) {
        batch.amplitudes[i] *= std::exp(-decay * deltaTime);
        batch.phases[i] += batch.frequencies[i] * deltaTime;
        // Update pre-calculated trig values
        batch.sinValues[i] = std::sin(batch.phases[i]);
        batch.cosValues[i] = std::cos(batch.phases[i]);
    }
}

 void AdaptiveGridRefinement::refineAtVertex(const vec3& vertex, const vec3& normal) {
    // Create additional samples around vertex
    auto samples = generateRefinedSamples(vertex);
    
    // Project samples into volume if needed
    if (m_config.useInternalSources) {
        for (auto& sample : samples) {
            sample += normal * m_config.normalOffset;
        }
    }
    
    // Add temporary wave sources
    for (const auto& sample : samples) {
        m_temporaryWaveSources.emplace_back(
            sample,
            m_config.dissipationRadius
        );
    }
}

} // namespace hd