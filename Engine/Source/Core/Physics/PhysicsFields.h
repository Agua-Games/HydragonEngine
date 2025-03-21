/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Baseline physics fields for representing physical properties, states, entities, in WavePhysics.
 * 
 * ARCHITECTURAL NOTES:
 *      - We're fundamentally treating everything through an energy-based approach.
 *      - Energy is the most fundamental quantity in physics, and everything else is derived from it.
 *      - Fields, along with Waves, are the core building blocks in WavePhysics. 
 *      - Fields basically always represent energy distributions in space, be it potential or converted: kinetic/momentum, etc, and in one form or another
 *      (of properties, which can be converted to other forms of energy, like kinetic, thermal, etc).
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

// === Generic Fields ===

// We leverage the concept of Energy Dimension (or Energy Spectrum), to map energy levels and states to topological variations.
// Dimensional Energy: Energy represented as a 4D vector (spectral components), which we can project to scalar when needed for simple calculations.
class EnergySpectrumField {
    Field<vec4, 3> energySpectrum;
};

/**
 * @brief Baseline generic field for force
 */
class ForceField {
    Field<vec3, 3> force;                 // Forces are computed as gradients/derivatives of potential energy fields
    Field<float, 3> potential;            // Potential energy field
};

/** 
 * @brief Baseline flexible and general field for energy, covering most energy manifestations which define physics properties, states, entities.
 * By combining these (sub)fields, we can simulate a wide range of physical phenomena. And we can always use them alone, as needed.
 */
class EnergyField {
    Field<vec3, 3> electricPotential;     // Electric potential energy
    Field<vec3, 3> magneticPotential;     // Magnetic potential energy
    Field<float, 3> nucleusEnergy;        // Nuclear binding energy + rest energy (E=mc²). Essential for Solid (body)Physics.
    Field<float, 3> density;              // Energy density distribution
    Field<float, 3> stability;            // State coupling energy
    Field<vec3, 3> gravity;               // Gravitational potential energy
    Field<float, 3> temperature;          // Thermal energy
    Field<float, 3> pressure;             // Pressure-volume energy
};

// === Specialized Fields ===

class EMField {
    Field<vec3, 3> electricPotential;     // Electric potential energy
    Field<vec3, 3> magneticPotential;     // Magnetic potential energy
    Field<float, 3> chargeDensity;        // Charge energy density
    Field<float, 3> currentDensity;       // Current energy density
};

class FluidField {
    Field<float, 3> density;
    Field<vec3, 3> velocity;
    Field<float, 3> pressure;
    Field<float, 3> temperature;
    bool isCompressible;                 // gas vs liquid behavior
};

class SolidField {
    Field<float, 3> density;
    Field<vec3, 3> stress;
    Field<float, 3> temperature;
    Field<float, 3> elasticity;         // Energy necessary to deform the material
    Field<float, 3> crystallineEnergy;  // Energy necessary to break crystal lattice
};

class PlasmaField {
    Field<float, 3> ionization;         // Energy necessary to ionize atoms
    Field<vec3, 3> density;
    Field<float, 3> temperature;
    // References to EMField for interactions
};

class SuperconductorField : public SolidField {
    Field<float, 3> cooperPairDensity;          // Energy necessary to break Cooper pairs
    Field<float, 3> criticalTemperature;        // Thermal energy threshold at which superconductivity occurs
};

// For advanced use cases
class QuantumField {
    Field<float, 3> energyLevel;
    Field<float, 3> energyDensity;
    Field<vec3, 3> phase;         // Energy phase
    Field<vec4, 3> energySpectrum;
}

} // namespace hd
