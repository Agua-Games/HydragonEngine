/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EnergyDimension.h
 * @brief Header file for the EnergyDimension class.
 * 
 * ARCHITECTURAL NOTES:
 * - EnergyDimension is a class that represents the energy dimension in the wave physics system.
 * - It's an experimental novel way of treating energy, conceptually, in a physics system. This allows us to map the energy dimension to 
 * topological variations (using a noise field, possibly 4d), to "explain" unexpected effects and phenomena related to energy levels and energy states, like quantum effects.
 * - Just to make sure, for users, specially more critical ones: this is not intended as a strict scientific approach, ambitious research or anything like that. It's used
 * as an experimental way to introduce quantum-like variations related to energy levels of waves/particles, specially in the behavior of high-energy waves, and also
 * extremely low energy scenarios, like those related to superconductors, black bodies, etc. and for this character, this feature is prone to change a lot as we further 
 * develop WavePhysics.
 * 
 * @todo Study deeper quantum effects related to high energy states of waves/particles, to introduce some reasonably modeled approximations, at least in terms of results.
 */
#pragma once
//#include <glm/gtx/vector_angle.hpp>
#include "PhysicsFields.h"
#include "Wave.h"

namespace hd {

/**
 * @brief EnergyDimension handles the energy dimension in the wave physics system.
 */
class EnergyDimension {
public:
    // === Structure Definitions ===
    struct ExcitationField {
        // 4D noise field (3D space + energy dimension)
        float sampleField(const vec3& position, float energyLevel) const;

        // Optimization: Cache frequently accessed regions
        struct CachedRegion {
            BoundingBox bounds;
            float energyMin, energyMax;
            Grid3D<float> samples;
        };
        
        std::vector<CachedRegion> m_cachedRegions;
    };

    // Energy level as a dimensional value
    struct EnergySpectrum {
        vec4 spectralComponents;  // Different energy manifestations
        
        // Project to scalar when needed
        float toScalar() const {
            return glm::dot(spectralComponents, 
                            vec4(0.2f, 0.3f, 0.3f, 0.2f));  // Weighted projection
        }
        
        // Combine with field modulation
        EnergySpectrum modulate(float fieldValue) const {
            // Non-linear modulation based on field strength
            float modFactor = std::pow(1.0f + std::abs(fieldValue), 2.0f);
            return EnergySpectrum{spectralComponents * modFactor};
        }
    };

    // Wave interaction with energy dimension
    struct ModulatedWave {
        WaveProperty baseProperties;
        EnergySpectrum energySpectrum;
        
        // Compute interaction with energy field
        void interact(const ExcitationField& field, const vec3& position);
    };

    // === Processing ===
    // Propagate waves through energy landscape (energy dimension topography)
    void propagateModulatedWave(const vec3& origin, 
                                const vec3& direction, 
                                const EnergySpectrum& initialEnergy);

private:
    // === Allocation, Initialization, Loading ===
    ExcitationField m_excitationField;
    float m_baseFrequency = 1.0f;
    float m_energyScale = 0.1f;
    float m_warpFactor = 0.5f;
    int m_octaves = 4;
    
    // === Processing ===
    /**
     * @brief Trigger quantum effects based on high energy levels, like particle decay, pair production, etc.
     * Intended for advanced use cases and simulations, or creative workflows.
     */
    void triggerQuantumEffects(const EnergySpectrum& energy);
};

} // namespace hd