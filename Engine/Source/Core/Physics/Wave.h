/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Wave.h
 * @brief Header file for the Wave struct and its LOD versions. It also includes specialized waves for specific scenarios.
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
 */
#pragma once
//#include <glm/gtc/matrix_transform.hpp>

namespace hd {

// === Base Wave ===
struct alignas(32) Wave {
    float amplitude;
    float frequency;
    float phase;
    vec3 propagation;

    // Flags for LOD/encoding
    uint32_t flags;  // Include encoding type, LOD level, etc.
};

/**
 * @brief WaveDeltaLOD1 represents a delta-encoded wave for Level 1 precision.
 * These LOD versions use delta compression approach. At first we think of using them for lower traveling range waves, like:
 * - Standing waves for solid and fluid fields
 * - Short-lived waves from Energy Transfer events
 * But possibly with the proper strategies we may use them for longer range waves too. Level 1: Highest precision, needs full alignment for efficient SIMD
 */
struct alignas(16) WaveDeltaLOD1 {
    // No flags in this level, to maintain proper alignment. We may need strategies to compensate
    uint32_t waveIndex : 24;
    uint8_t deltaAmplitude : 4;
    uint8_t deltaFrequency : 4;
    vec3_packed propagation;  // Assuming vec3_packed needs 16-byte alignment
};

// Level 2: Medium precision, can use smaller alignment
struct alignas(8) WaveDeltaLOD2 {
    uint32_t waveIndex : 24;
    uint8_t deltaAmplitude : 3;
    uint8_t deltaFrequency : 3;
    uint8_t flags : 2;
    vec3_packed8 propagation;  // Assuming vec3_packed8 uses less precision
};

// Level 3: Lowest precision, minimal alignment needed
struct alignas(4) WaveDeltaLOD3 {
    uint32_t waveIndex : 24;
    uint8_t deltaAmplitude : 2;
    uint8_t deltaFrequency : 2;
    uint8_t flags : 2;
    vec3_packed4 propagation;  // Lowest precision vec3
};

// === Acoustic Wave ===
struct AcousticWave : Wave {
    // Derived properties (calculated from base Wave)
    float intensity() const { 
        return amplitude * amplitude; // Sound intensity ∝ amplitude²
    }
    
    float soundPressureLevel() const {
        return 20.0f * log10(amplitude / REFERENCE_PRESSURE);
    }
    
    float wavelength() const {
        return SPEED_OF_SOUND / frequency;
    }

    // Additional acoustic-specific properties
    float resonance;
    float impedance;      // Acoustic impedance of medium
    
    struct CharacterProperties {
        float brightness;  // Derived from frequency spectrum
        float roughness;   // From phase relationships
        float warmth;      // From lower frequency content
    };

    // Calculate sound character at listener position
    CharacterProperties calculateCharacter(const AcousticListener& listener) const {
        float distanceAttenuation = calculateDistanceAttenuation(listener.position);
        float mediumAbsorption = calculateMediumAbsorption(listener.position);
        
        return {
            calculateBrightness(frequency, distanceAttenuation),
            calculateRoughness(phase, amplitude),
            calculateWarmth(frequency, mediumAbsorption)
        };
    }
};

struct alignas(16) AcousticWaveDeltaLOD1 : WaveDeltaLOD1 {
    // Derived properties (calculated from base Wave)
    float intensity() const { 
        return amplitude * amplitude; // Sound intensity ∝ amplitude²
    }
    
    float soundPressureLevel() const {
        return 20.0f * log10(amplitude / REFERENCE_PRESSURE);
    }
    
    float wavelength() const {
        return SPEED_OF_SOUND / frequency;
    }

    // Additional acoustic-specific properties
    float resonance;
    float impedance;      // Acoustic impedance of medium
    
    struct CharacterProperties {
        float brightness;  // Derived from frequency spectrum
        float roughness;   // From phase relationships
        float warmth;      // From lower frequency content
    };

    // Calculate sound character at listener position
    CharacterProperties calculateCharacter(const AcousticListener& listener) const {
        float distanceAttenuation = calculateDistanceAttenuation(listener.position);
        float mediumAbsorption = calculateMediumAbsorption(listener.position);
        
        return {
            calculateBrightness(frequency, distanceAttenuation),
            calculateRoughness(phase, amplitude),
            calculateWarmth(frequency, mediumAbsorption)
        };
    }
};

// Level 2: Medium precision, can use smaller alignment
struct alignas(8) AcousticWaveDeltaLOD2 : WaveDeltaLOD2 {
    // Derived properties (calculated from base Wave)
    float intensity() const { 
        return amplitude * amplitude; // Sound intensity ∝ amplitude²
    }
    
    float soundPressureLevel() const {
        return 20.0f * log10(amplitude / REFERENCE_PRESSURE);
    }
    
    float wavelength() const {
        return SPEED_OF_SOUND / frequency;
    }

    // Additional acoustic-specific properties
    float resonance;
    float impedance;      // Acoustic impedance of medium
    
    struct CharacterProperties {
        float brightness;  // Derived from frequency spectrum
        float roughness;   // From phase relationships
        float warmth;      // From lower frequency content
    };

    // Calculate sound character at listener position
    CharacterProperties calculateCharacter(const AcousticListener& listener) const {
        float distanceAttenuation = calculateDistanceAttenuation(listener.position);
        float mediumAbsorption = calculateMediumAbsorption(listener.position);
        
        return {
            calculateBrightness(frequency, distanceAttenuation),
            calculateRoughness(phase, amplitude),
            calculateWarmth(frequency, mediumAbsorption)
        };
    }
};

// Level 3: Lowest precision, minimal alignment needed
struct alignas(4) AcousticWaveDeltaLOD3 : WaveDeltaLOD3 {
    // Derived properties (calculated from base Wave)
    float intensity() const { 
        return amplitude * amplitude; // Sound intensity ∝ amplitude²
    }
    
    float soundPressureLevel() const {
        return 20.0f * log10(amplitude / REFERENCE_PRESSURE);
    }
    
    float wavelength() const {
        return SPEED_OF_SOUND / frequency;
    }

    // Additional acoustic-specific properties
    float resonance;
    float impedance;      // Acoustic impedance of medium
    
    struct CharacterProperties {
        float brightness;  // Derived from frequency spectrum
        float roughness;   // From phase relationships
        float warmth;      // From lower frequency content
    };

    // Calculate sound character at listener position
    CharacterProperties calculateCharacter(const AcousticListener& listener) const {
        float distanceAttenuation = calculateDistanceAttenuation(listener.position);
        float mediumAbsorption = calculateMediumAbsorption(listener.position);
        
        return {
            calculateBrightness(frequency, distanceAttenuation),
            calculateRoughness(phase, amplitude),
            calculateWarmth(frequency, mediumAbsorption)
        };
    }
};

// === Electromagnetic Wave ===
struct EMWave : Wave {
    // === Field Components ===
    vec3 electricField;    // E-field vector
    vec3 magneticField;    // B-field vector (always perpendicular to E and propagation)
    float polarization;    // Polarization angle in radians
    
    // === Derived Properties ===
    float wavelength() const { 
        return SPEED_OF_LIGHT / frequency; 
    }
    
    float energy() const {
        return PLANCK_CONSTANT * frequency;  // E = hf
    }
    
    float intensity() const {
        // I = ε₀c|E|²/2, where ε₀ is vacuum permittivity
        return VACUUM_PERMITTIVITY * SPEED_OF_LIGHT * 
               glm::length2(electricField) * 0.5f;
    }

    // === Interaction Properties ===
    struct MaterialInteraction {
        float permittivity;     // Relative permittivity (εᵣ)
        float permeability;     // Relative permeability (μᵣ)
        float conductivity;     // Electrical conductivity (σ)
        
        // Derived properties
        float impedance() const {
            return std::sqrt(permeability / permittivity) * VACUUM_IMPEDANCE;
        }
        
        float skinDepth() const {
            // Depth where field reduces to 1/e
            return std::sqrt(2.0f / (conductivity * permeability * frequency));
        }
        
        float attenuationCoeff() const {
            return std::sqrt(M_PI * frequency * conductivity * permeability);
        }
    } material;

    // === Wave Behavior ===
    struct PropagationEffects {
        float phaseVelocity;    // v = c/n (n = refractive index)
        float groupVelocity;    // dω/dk (usually ≤ c)
        float dispersion;       // Frequency-dependent velocity changes
        float diffraction;      // Bending around obstacles
        
        struct Interfaces {
            float reflectivity;     // Fresnel reflection coefficient
            float transmissivity;   // Transmission coefficient
            float refractiveIndex;  // n = c/v
        } interfaces;
    } propagation;

    // === Quantum Properties ===
    struct QuantumEffects {
        bool canPhotoionize;        // Energy > ionization threshold
        bool canInducePairProduction; // E > 2mc²
        float comptonWavelength;    // λ = h/mc
        float photoelectricWork;    // Work function threshold
    } quantum;

    // === Methods ===
    void updateFields() {
        // E and B fields are perpendicular to propagation and each other
        vec3 normalizedProp = glm::normalize(propagation);
        
        // Create orthonormal basis
        vec3 eDirection = glm::rotate(
            vec3(1.0f, 0.0f, 0.0f), 
            polarization,
            normalizedProp
        );
        
        // B field completes right-handed system
        vec3 bDirection = glm::cross(normalizedProp, eDirection);
        
        // Update fields with current amplitude
        electricField = eDirection * amplitude;
        magneticField = bDirection * (amplitude / SPEED_OF_LIGHT);
    }

    float calculateReflection(const MaterialInteraction& interface) const {
        float Z1 = VACUUM_IMPEDANCE;
        float Z2 = interface.impedance();
        float r = (Z2 - Z1) / (Z2 + Z1);
        return r * r;  // Power reflection coefficient
    }

    void propagateThrough(const MaterialInteraction& medium) {
        // Update amplitude based on attenuation
        float distance = 1.0f;  // Use actual distance in implementation
        float attenuation = std::exp(-medium.attenuationCoeff() * distance);
        amplitude *= attenuation;
        
        // Update phase based on medium properties
        float phaseShift = 2.0f * M_PI * distance * frequency * 
                          std::sqrt(medium.permittivity * medium.permeability) / 
                          SPEED_OF_LIGHT;
        phase += phaseShift;
        
        // Maintain field orthogonality
        updateFields();
    }
};

// LOD1: High precision, full alignment
struct alignas(16) EMWaveDeltaLOD1 : WaveDeltaLOD1 {
    vec3_packed electricField;
    vec3_packed magneticField;
    float polarization;
    
    struct MaterialInteraction {
        float permittivity;
        float permeability;
        float conductivity;
    } material;
};

// LOD2: Medium precision, reduced properties
struct alignas(8) EMWaveDeltaLOD2 : WaveDeltaLOD2 {
    vec3_packed8 electricField;    // Reduced precision
    uint8_t polarizationQuantized; // Quantized to 256 values
    
    struct MaterialInteraction {
        uint8_t permittivity : 3;  // Quantized material properties
        uint8_t permeability : 3;
        uint8_t conductivity : 2;
    } material;
    
    // Magnetic field can be derived from electric field
    vec3_packed8 deriveMagneticField() const {
        return calculateMagneticField(electricField, propagation);
    }
};

// LOD3: Minimal precision, essential properties only
struct alignas(4) EMWaveDeltaLOD3 : WaveDeltaLOD3 {
    vec3_packed4 electricField;    // Minimal precision
    uint8_t properties;            // Packed properties:
    // bits 0-2: polarization (8 discrete angles)
    // bits 3-4: material type (4 preset materials)
    // bits 5-7: reserved
    
    // All other properties derived from base wave and presets
};

// === Ggravitational Wave ===
struct GravitationalWave : Wave {
    // For space/gravity-based games
    float spacetimeDistortion;
    vec3 quadrupoleMoment;
};

struct alignas(16) GravitationalWaveDelta1 : WaveDeltaLOD1 {
    // For space/gravity-based games
    float spacetimeDistortion;
    vec3_packed quadrupoleMoment;
};

// Level 2: Medium precision, can use smaller alignment
struct alignas(8) GravitationalWaveDelta2 : WaveDeltaLOD2 {
    // For space/gravity-based games
    uint8_t spacetimeDistortion;
    vec3_packed8 quadrupoleMoment;
};

// Level 3: Lowest precision, minimal alignment needed
struct alignas(4) GravitationalWaveDelta3 : WaveDeltaLOD3 {
    // For space/gravity-based games
    uint8_t spacetimeDistortion;
    vec3_packed4 quadrupoleMoment;
};

// === Quantum Wave ===
struct QuantumWave : Wave {
    // For quantum effects in sci-fi scenarios
    float probabilityDensity;
    float spinState;
    bool entangled;
};

struct alignas(16) QuantumWaveDelta1 : WaveDeltaLOD1 {
    /// For quantum effects in sci-fi scenarios
    float probabilityDensity;
    float spinState;
    bool entangled;
};

// Level 2: Medium precision, can use smaller alignment
struct alignas(8) QuantumWaveDelta2 : WaveDeltaLOD2 {
    // For quantum effects in sci-fi scenarios
    uint8_t probabilityDensity;
    uint8_t spinState;
    bool entangled;
};

// Level 3: Lowest precision, minimal alignment needed
struct alignas(4) QuantumWaveDelta3 : WaveDeltaLOD3 {
    // For quantum effects in sci-fi scenarios
    uint8_t probabilityDensity;
    uint8_t spinState;
    bool entangled;
};

// === Chemical Wave ===
struct ChemicalWave : Wave {
    // For reaction propagation in materials
    float concentration;
    float reactionRate;
    uint16_t reactantMask;
};

struct alignas(16) ChemicalWaveDelta1 : WaveDeltaLOD1 {
    // For reaction propagation in materials
    float concentration;
    float reactionRate;
    uint16_t reactantMask;
};

// Level 2: Medium precision, can use smaller alignment
struct alignas(8) ChemicalWaveDelta2 : WaveDeltaLOD2 {
    // For reaction propagation in materials
    uint8_t concentration;
    uint8_t reactionRate;
    uint8_t reactantMask;
};

// Level 3: Lowest precision, minimal alignment needed
struct alignas(4) ChemicalWaveDelta3 : WaveDeltaLOD3 {
    // For reaction propagation in materials
    uint8_t concentration;
    uint8_t reactionRate;
    uint8_t reactantMask;
};

} // namespace hd
