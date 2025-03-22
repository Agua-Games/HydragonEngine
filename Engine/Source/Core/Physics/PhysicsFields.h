/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Physics fields for representing potential or converted/manifested energy distributions in space, of various forms. 
 * This is the core of our physics architecture.
 * 
 * ARCHITECTURAL NOTES:
 *      - We're fundamentally treating everything through an energy-based approach.
 *      - Energy is the most fundamental quantity in physics, and everything else is derived from it.
 *      - Fields, along with Waves, are the core building blocks in WavePhysics. 
 *      - Fields basically always represent energy distributions in space, be it potential or converted: kinetic/momentum, etc, and in one form or another
 *      (of properties, which can be converted to other forms of energy, like kinetic, thermal, etc).
 *      - So, basically, at the heart of our architecture we have:
 * 
 *          Energy (the quantity/data) --> Wave (the carrier pattern: excitation/perturbation/radiation in a field) --> Field (the set/system: energy distribution in space)
 * 
 *      - We store Energy, derive Forces:
 *          - Fields should store energy values (potential and kinetic).
 *          - Forces can be computed as gradients/derivatives of energy fields.
 *          - This aligns with fundamental physics (forces as gradients of potential energy).
 *          - More fundamental (energy conservation is a fundamental law)
 *          - Easier to handle energy transfer between different field types (leads to Field inter-compatibility).
 *          - Better aligns with wave-based physics (waves carry energy).
 *          - More natural handling of potential vs kinetic energy transformations.
 *      - To avoid confusion: we're declaring classes with the suffix "Field" here, but those are consciously not meant to inherit from the base Field class in Field.h.
 *      The VARIABLES using the Field class as data type are declared, bundled inside of each [physical_property]Field class.
 */
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <complex>
#include <array>
#include "Field.h"

namespace hd {
// === Helper structs ===
/**
 * @brief Helper struct for handling field-to-field interactions
 */
struct FieldCoupling {
    float couplingEnergy;          // Energy exchange coefficient
    float interactionStrength;     // How strongly fields influence each other
};

/**
 * @brief Properties of the medium the field exists in
 */
struct MediumProperties {
    float wavePropagationSpeed;     // Speed of waves in this medium
    float energyDissipationRate;    // How quickly energy dissipates
    float impedance;                // Medium's resistance to wave propagation
};

// === Generic Fields ===

/**
 * @brief We leverage the concept of Energy Dimension (or Energy Spectrum), to map energy levels and states to topological variations.
 * Dimensional Energy: Energy represented as a 4D vector (spectral components), which we can project to scalar when needed for simple calculations.
 */
struct EnergySpectrum {
    Field<vec4, 3> energySpectrum;      // Different energy manifestations
};

/**
 * @brief Baseline generic field for force
 */
struct ForceField {
    Field<vec3, 3> force;                 // Forces are computed as gradients/derivatives of potential energy fields
    Field<float, 3> potential;            // Potential energy field
};

/** 
 * @brief Baseline flexible and general field for energy, covering most energy manifestations which define physics properties, states, entities.
 * By combining these (sub)fields, we can simulate a wide range of physical phenomena. And we can always use them alone, as needed.
 */
struct EnergyField {
    // Fundamental force-carrier fields (representing interaction energies)
    Field<vec3, 3> electricPotential;     // Electric potential energy
    Field<vec3, 3> magneticPotential;     // Magnetic potential energy
    Field<float, 3> nucleusPotential;     // Nuclear binding energy + rest mass potential energy (E=mc²). Essential for Solid (body)Physics.
    Field<vec3, 3> gravityPotential;      // Gravitational potential energy
    
    // State-defining energy fields
    Field<float, 3> energyDensity;        // Energy density distribution    (particle-level kinetic energy)
    Field<float, 3> couplingEnergy;       // Inter-particle binding energy  (particle-level kinetic energy)
    Field<float, 3> thermalEnergy;        // Thermal energy                 (particle-level kinetic energy)
    Field<float, 3> pressureEnergy;       // Pressure-volume energy         (particle-level kinetic energy)
    Field<vec3, 3> momentumEnergy;        // Momentum energy                (particle-level kinetic energy)
};

// === Specialized Fields ===

struct EMField {
    Field<vec3, 3> electricPotential;     // Electric potential energy
    Field<vec3, 3> magneticPotential;     // Magnetic potential energy
    Field<float, 3> chargeEnergy;         // Charge-based potential energy
    Field<float, 3> currentEnergy;        // Moving charge energy

    // Compute forces from energy gradients
    vec3 getElectricForce(const vec3& position) const {
        return -electricPotential.computeGradient(position);
    }

    vec3 getMagneticForce(const vec3& position) const {
        return -magneticPotential.computeGradient(position);
    }

    FieldCoupling getFieldCoupling(const EMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

struct GravitationalField {
    Field<float, 3> potentialEnergy;    // Gravitational potential energy
    Field<float, 3> energyDensity;      // Energy density
    Field<float, 3> massDensity;        // Mass density

    // Compute forces from energy gradients
    vec3 getGravitationalForce(const vec3& position) const {
        return -potentialEnergy.computeGradient(position);
    }

    FieldCoupling getFieldCoupling(const GravitationalField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

struct FluidField {
    Field<float, 3> energyDensity;
    Field<vec3, 3> kineticEnergy;        // Particle-level kinetic energy with velocity
    Field<float, 3> potentialEnergy;     // Particle-level potential energy
    Field<vec3, 3> momentumEnergy;       // Momentum energy                (particle-level kinetic energy)
    Field<float, 3> pressureEnergy;      // Compression-expansion energy. Particle-level kinetic energy
    Field<float, 3> thermalEnergy;       // Particle-level kinetic energy

    bool isCompressible;                 // Gas vs liquid behavior. Compressibility is a property of the medium.
    MediumProperties medium;             // Properties of the medium the field exists in

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const EMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

// Advanced fluid simulation with detailed energy interactions
struct AdvancedFluidField : public FluidField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;         // Internal friction energy
    Field<float, 3> surfaceTensionEnergy;    // Surface energy
    Field<float, 3> vorticalEnergy;          // Rotational flow energy. Like in turbulence.
    Field<float, 3> turbulenceEnergy;        // Chaotic flow energy.
};

// Computational/high-detail fluid simulation with detailed energy interactions
struct ComputationalFluidField : public AdvancedFluidField {
    // Additional computational properties
    Field<float, 3> cavitationEnergy;         // Vacuum bubble energy
    Field<float, 3> boundaryLayerEnergy;      // Detailed wall energy
    Field<float, 3> shockWaveEnergy;          // High-pressure wave energy
    Field<float, 3> acousticEnergy;           // Sound wave energy
};

struct GasField : public FluidField {
    Field<float, 3> viscosityEnergy;     // Frictional energy
};

// Advanced gas simulation with molecular-level energetics
struct AdvancedGasField : public FluidField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;      // Frictional energy
    Field<vec3, 3> convectionEnergy;      // Heat flow energy.
    Field<float, 3> diffusionEnergy;      // Mixing energy
};

// Computational/high-detail gas simulation with molecular-level energetics
struct ComputationalGasField : public AdvancedFluidField {
    // Additional computational properties
    Field<float, 3> entropyField;         // Disorder energy
    Field<float, 3> chemicalEnergy;       // Chemical reaction energy
    Field<float, 3> phaseTransitionEnergy; // Phase change energy
};

struct SolidField {
    Field<float, 3> energyDensity;
    Field<vec3, 3> strainEnergy;             // Deformation energy
    Field<float, 3> thermalEnergy;           // Particle-level kinetic energy
    Field<float, 3> elasticEnergy;           // Recoverable deformation energy
    Field<float, 3> latticeEnergy;           // Crystalographic lattice energy

    MediumProperties medium;                 // Properties of the medium the field exists in

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const EMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

// Advanced solid simulation with crystalline and structural energetics
struct AdvancedSolidField : public SolidField {
    // Additional advanced properties
    Field<float, 3> anisotropicEnergy;    // Directional property energy
    Field<vec3, 3> residualStressEnergy;  // Internal stress energy
    Field<float, 3> crackedEnergy;        // Fracture surface energy
    Field<float, 3> fatigueEnergy;        // Cyclic loading energy
};

// Computational/high-detail solid simulation with crystalline and structural energetics
struct ComputationalSolidField : public SolidField {
    // Additional advanced properties
    Field<float, 3> dislocationEnergy;        // Crystal defects energy
    Field<float, 3> grainBoundaryEnergy;      // Crystal interface energy
    Field<float, 3> phononEnergy;             // Lattice vibration energy
    Field<float, 3> plasticEnergy;            // Permanent deformation energy
};

/**
 * @brief Plasma field, inheriting from EMField for electromagnetic interactions.
 */
struct PlasmaField : public FluidField, public EMField {
    Field<float, 3> viscosityEnergy;     // Frictional energy
    Field<float, 3> ionizationEnergy;         //Ionization energy
};

// Advanced Plasma simulation with molecular-level energetics
struct AdvancedPlasmaField : public FluidField, public EMField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;     // Frictional energy
    Field<float, 3> ionizationEnergy;         //Ionization energy
    Field<vec3, 3> convectionEnergy;      // Heat flow energy.
    Field<float, 3> diffusionEnergy;      // Mixing energy
};

struct SuperconductorField : public SolidField {
    Field<float, 3> cooperPairBindingEnergy;    // Energy necessary to break Cooper pairs
    Field<float, 3> criticalThermalEnergy;      // Thermal energy threshold at which superconductivity occurs
};

struct QuantumField {
    Field<float, 3> energyLevel;        // Quantum state energy
    Field<float, 3> energyDensity;      // Probability density * energy
    Field<vec3, 3> phaseEnergy;         // Wave function phase energy
    Field<vec4, 3> energySpectrum;      // Energy distribution across quantum states
};

struct HiggsField {
    Field<float, 3> potentialEnergy;    // Higgs potential energy
    Field<float, 3> energyDensity;      // Energy density
    Field<float, 3> massDensity;        // Mass density

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const HiggsField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

// For fiction, entertainment
struct MagicField : public QuantumField, public EMField, public FluidField {
    // Additional advanced properties
    Field<float, 3> magicPotential;     // Magic potential energy
    Field<float, 3> magicEnergyDensity; // Magic energy density
    Field<vec3, 3> healingPotential;    // Healing potential energy

    // Magic-specific methods
    void castSpell(const Spell& spell) {
        // Implement spell casting logic
    }

    void createAura(const Aura& aura) {
        // Implement aura creation logic
    }

    void createHealingField(const Healing& healing) {
        // Implement healing logic
    }

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const MagicField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

// For fiction, entertainment - Advanced version. Enhanced with some advanced fluid properties, like vorticity, turbulence, etc, for richer
// visuals.
struct AdvancedMagicField : public QuantumField, public EMField, public AdvancedFluidField {
    // Additional advanced properties
    Field<float, 3> magicPotential;     // Magic potential energy
    Field<float, 3> magicEnergyDensity; // Magic energy density
    Field<vec3, 3> healingPotential;    // Healing potential energy

    std::vector<vec3> spellTargets;

    vec3 getMagicGradient(const vec3& position) const {
        // Implement gradient calculation logic
        return vec3(0.0f);
    }

    // Magic-specific methods
    void castSpell(const Spell& spell) {
        // Implement spell casting logic
    }

    void createAura(const Aura& aura) {
        // Implement aura creation logic
    }

    void createHealingField(const Healing& healing) {
        // Implement healing logic
    }

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const MagicField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

struct ExtremePhysicsField : public QuantumField, public EMField, public AdvancedFluidField {
    // Properties are expected to have borderline values, unexpected energy interactions and chain reactions, most of them harmful to
    // structures with predisposition to chemical reactions, decay through radiation levels, etc. One of the most unpredictable and
    // creative fields/scenarios.
    Field<float, 3> extremeEnergyDensity; // Energy density that can cause chain reactions
    Field<vec3, 3> extremeGradient;        // Gradient that can cause instabilities
    Field<float, 3> extremePotential;     // Potential that can cause instabilities

    EnergySpectrum extremeEnergySpectrum;

    // References to EMField for interactions    
    FieldCoupling getFieldCoupling(const ExtremePhysicsField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }

    // Methods to handle extreme physics
    void handleInstability(const vec3& position) {
        // Implement instability handling logic
    }

    void handleChainReaction(const vec3& position) {
        // Implement chain reaction handling logic
    }
}

} // namespace hd
