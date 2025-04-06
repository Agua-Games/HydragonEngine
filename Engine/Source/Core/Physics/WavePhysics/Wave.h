/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Wave.h
 * 
 * ARCHITECTURAL NOTES:
 * - Wave is a struct that represents a wave in WavePhysics's physics simulation.
 * - It contains properties such as amplitude, frequency, phase, and propagation direction.
 * - Base Wave:
 *      - Short-lived energy transfers (impacts, explosions)
 *      - Standing waves (solid/fluid fields)
 *      - General purpose energy propagation
 *  
 * - AcousticWave:
 *      - Sound propagation
 *      - Pressure waves
 *      - Mechanical vibrations
 *  
 * - EMWave:
 *      - Light
 *      - Heat radiation
 *      - Electric/magnetic fields
 *      - Radio waves
 *      - Particle decay effects
 * - Other waves for specific scenarios include: GravitationalWave, PlasmaWave, etc.
 * - Wave is one of the few most important/essential components which forms the basis of the whole WavePhysics system.
 * - One of the paramount things to consider for Wave is that it should be thoroughly designed to account for both memory optimization and essential functionality. The memory
 * optimization side is mainly because of the fact that we expect hundreds to thousands of waves per scene. So Wave should be as compact as possible.
 * - For now, we're storing some wave properties very tightly in the "properties" member. The specific properties and amount of bits allocated to each property is subject 
 * to change, after extensive tests.
 * - To derive particle properties from Wave, we can use the following formula: 
 *      particle.momentum = wave.amplitude * wave.frequency * wave.propagation;
 *      particle.energy = wave.amplitude * wave.frequency;
 * 
 * - We also have the Ray class, which is a much simpler data structure than Wave, and not physics-oriented, but can be used for raytracing/raycasting, and for being much simpler
 * is much more optimized. Also, different than Wave, which obeys the laws of physics (speed of light, etc), Ray is by default instantaneous (from source to end of length). Though,
 * we can force it to obey speed of light and some other constraints, to model cheap light rays, sound rays, etc.
 * 
 * @todo After tests with solid working results, gradually move the content from Wave_Legacy.h to here, ALWAYS TESTING, IN SMALL STEPS.
 * It's paramount to not forget to study Wave_Legacy.h again and again, and perform this migration, because there's very useful content there.
 */
#pragma once
#include <glm/glm.hpp>

/**
 * @brief Wave represents a wave in WavePhysics's physics simulation.
 * For Wave, it's paramount to be tight on memory and at the same time store the greatest amount of properties, specially the most essential ones, all the while keeping
 * the 32-byte alignment.
 * See the Architectural Notes for Wave.h (above) for more details, like how to derive particle properties from Wave.
 */
struct alignas(32) Wave {
    float amplitude;        // 4 bytes: Primary energy carrier
    float frequency;        // 4 bytes: Oscillation rate
    float phase;            // 4 bytes: Current phase
    vec3 propagation;       // 12 bytes: Direction + magnitude combined
    
    uint32_t flags;         // 4 bytes packed as:
        // bits 0-7:   encoding type
        // bits 8-15:  LOD level
        // bits 16-23: lifetime
        // bits 24-31: interaction radius

    // Hybrid packed properties
    uint32_t properties;   // 4 bytes hybrid packed as:
        // bits 0-3:   wave type (EMWave, AcousticWave, etc)
        // bits 4-11:  impedance (medium resistance)
        // bits 12-15: attenuation rate
        // bits 16-19: dispersion factor
        // bits 20-23: material ID
        // bits 24-27: decay behavior
        // bits 28-31: penetration depth
};  // Total: 32 bytes, perfectly aligned for SIMD