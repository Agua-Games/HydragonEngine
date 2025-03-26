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
 *      In WavePhysics, we model energy, fundamentally, as (nothing more, let's say, than) a tensor between fields. This is a very powerful and elegant approach,
 *      which allows us to model energy transfer and transformation in a very natural and elegant way, and also to handle energy transfer between different field types.
 *      So, for instance, in E = mc², mass is the energy tensor between the fields of mass and energy. Or,  we could also say, the strength of the coupling between the
 *      wave and the underlying field. The way this coupling works has a function curve yet to be found, because when they are multiplied together their range becomes
 *      exponentially longer in an extreme function curve - that's why gravity is so weak and goes so far, and the opposite is true for the nuclear forces. So: in an
 *      extreme, we have almost massless waves (we could call them particles also, like the photon. Massless meaning, in other words: they have among the weakest of
 *      couplings with the underlying field, and so their coupling to the field is so weak that they display quantum-like behaviors, which means that they can seem to
 *      "jump" in some fields, this property directly related to their energy levels vs mass levels) and in the other extreme we have supermassive blackholes, etc 
 *      (these, by the effect where the joining of the masses of its constituents cause this "super-exponential" range curve, start to have the inverse relationship that
 *      the almost massless high-energy particles have with the umderlying field: instead of possibly showing skips in relation to the field/being almost un-coupled from
 *      it, the coupling is so strong that they distort the relationship with the field towards enormous vectors - extreme gravity levels, etc). Continuing: c², the rest 
 *      mass, is the energy tensor between each wave's fields. So, as in the relativistic (we could say: relative momentum based) postulations if, for example, two waves 
 *      are "traveling" with exact the same momentum vector, they'll have zero (momentum) energy in relation to each other. So their divergence energy tensor is zero. So, 
 *      automatically we derive that the divergence energy tensor also can be enormous between fields, given the right circumstances and also is the most promising way to 
 *      convert extreme amounts of energy from one form to another, in any area of technology, from propulsion and other "man-enhancing" tech to destructive tech, weapons, etc.
 *
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
 *      - The base Field class, in Field.h, has functionality useful for all sorts of fields, worth checking, like computeAverage(), computeGradient(), etc.
 * 
 * TODO:
 *      - Get rid of LocalSampler struct after moving local adaptive refinement management to WavePhysics.h.
 *      - Decide if EnergySpectrum's spectral components should be a simple modulated scalar value, or a 4D vector (spectral components). Maybe the scalar choice is used
 *      for a LOD version.
 *      - Test and refactor struct EnergySpectrum so that its functions model the intended design (wave properties, like relationship with time being affected by the
 *      EnergySpectrum through the wave's energy level. The modulation factor uses a non-linear function, but we must find the right algorithm for it.) This will be useful to
 *      model quantum effects, among others.
 *      - After thourough tests, determine which AdvancedEMField properties will actually be used. Some may be moved to ComputationalEMField.
 *      - Same for Solid, AdvancedSolidField and ComputationalSolidField: decide which properties are Advanced or Compute Level. This task is just because of allocated memory
 *      optimization issues, when there will be many fields in a scene, possibly hundreds or thousands. For scientifical computation this issue is much less relevant and
 *      many times in fact they'll prefer more detail, for more reliable results.
 *      - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 *      - Flesh out the class and its methods, structs, enums, etc.
 */
#pragma once
#include <vector>
#include <cmath>
#include <string>
#include <memory>
#include <complex>
#include <array>
#include <glm/gtx/vector_angle.hpp>
#include "Field.h"
#include "Wave.h"

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
 * @brief We leverage the concept of Energy Dimension (a field arising from the mapping of one particle's energy field(s) to another's), to map energy levels and 
 * states to topological variations. Dimensional Energy: Energy represented as a 4D vector (spectral components), which we can project to scalar when needed for 
 * simple calculations.
 *  @todo update the mapping algorithm to account for the latest clarifications.
 */
struct EnergySpectrum {
    // Energy level as a dimensional value
    vec4 spectralComponents;  // Different energy manifestations
    
    // Project to scalar when needed
    float toScalar() const {
        return glm::dot(spectralComponents, 
                        vec4(0.2f, 0.3f, 0.3f, 0.2f));  // Weighted projection
    }
    
    // The spectral components are basically used to change the relationship of a wave (in a specific cell in a field) with time (the 4th dimension), based
    // on the energy level
    struct WaveProperties {
        float frequency;         // Oscillation frequency
        float wavelength;        // Spatial periodicity
        vec3 propagation;        // Direction of wave travel
        float phaseVelocity;    // Wave front speed
    };

    // Combine with field modulation
    EnergySpectrum modulate(float fieldValue) const {
        // Non-linear modulation based on field strength
        float modFactor = std::pow(1.0f + std::abs(fieldValue), 2.0f);
        return EnergySpectrum{spectralComponents * modFactor};
    }

    // Create base wave properties based on energy spectrum
    WaveProperties createBaseWaveProperties() const {
        // Convert spectral components to wave properties
        return WaveProperties{
            toScalar(),                  // Frequency
            1.0f / toScalar(),           // Wavelength
            vec3(0.0f, 0.0f, 1.0f),      // Propagation direction
            toScalar()                   // Phase velocity
        };
    }
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

    // Compute forces from energy gradients
    vec3 getForce(const vec3& position) const {
        return -electricPotential.computeGradient(position) -
               magneticPotential.computeGradient(position) -
               nucleusPotential.computeGradient(position) -
               gravityPotential.computeGradient(position);
    }

    FieldCoupling getFieldCoupling(const EnergyField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }

    EnergySpectrum getEnergySpectrum(const vec3& position) const {
        // Calculate energy spectrum based on field properties
        return EnergySpectrum{...};
    }

    // Modulate energy for a field using the energy spectrum.
    // Return the field property/specific energy modified by the energy spectrum.
    float modulateEnergy(const vec3& position, float fieldValue) const {
        EnergySpectrum spectrum = getEnergySpectrum(position);
        return spectrum.modulate(fieldValue).toScalar();
    }
    
    MediumProperties getMediumProperties(const vec3& position) const {
        // Calculate medium properties based on field properties
        return MediumProperties{...};
    }
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

    EnergySpectrum getEnergySpectrum(const vec3& position) const {
        // Calculate energy spectrum based on field properties
        return EnergySpectrum{...};
    }

    MediumProperties getMediumProperties(const vec3& position) const {
        // Calculate medium properties based on field properties
        return MediumProperties{...};
    }
};

// Advanced EM field with detailed energy interactions
struct AdvancedEMField {
    // Additional advanced properties
    float permeability;             // Similar to fluid viscosity
    float permittivity;             // Like fluid compressibility

    // Wave characteristics (shared with fluid waves)
    struct WaveProperties {
        float frequency;         // Oscillation frequency
        float wavelength;        // Spatial periodicity
        vec3 propagation;        // Direction of wave travel
        float phaseVelocity;    // Wave front speed
        
        struct Coupling {
            float EMcoupling;    // E-B field interaction
            float materialCoupling; // Field-matter interaction
            float resonance;     // Natural frequencies
        };
    };

    // Field dynamics similar to fluid turbulence
    struct FieldDynamics {
        vec3 poyntingVector;     // EM energy flow (cf. fluid momentum)
        float fieldEnergy;       // Energy density
        vec3 magneticVorticity;  // Like fluid vorticity
        float divergence;        // Field source/sink (Gauss's law)
        
        struct Currents {
            vec3 eddyCurrent;    // Similar to fluid eddies
            vec3 displacement;    // Changing E-field contribution
            float conductivity;   // Material response to fields
            float hallEffect;    // Charge carrier deflection
        };
    };

    // Material interaction (parallel to fluid-material interaction)
    struct MaterialResponse {
        struct Magnetic {
            float susceptibility;  // Like fluid susceptibility
            float remanence;      // Residual magnetization
            float coercivity;     // Field resistance
            bool isParamagnetic;  // Material type flag
        };

        struct Electric {
            float polarizability; // Like fluid polarizability
            float capacitance;    // Charge storage
            float resistance;     // Current opposition
            bool isDielectric;    // Material type flag
        };
    };

    FieldCoupling getFieldCoupling(const AdvancedEMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

/**
 * @brief Gravitational field. Due to gravity being such a long range and weak force, we treat it separately, in general skipping the gravitationalEnergy in other fields,
 * using a downwards term applied over momentumPotential.
 */
struct GravitationalField {
    Field<float, 3> gravitationalPotential;    // Gravitational potential energy
    Field<float, 3> energyDensity;            // Energy density
    Field<float, 3> massDensity;              // Mass density

    // Compute forces from energy gradients
    vec3 getGravitationalForce(const vec3& position) const {
        return -gravitationalPotential.computeGradient(position);
    }

    FieldCoupling getFieldCoupling(const GravitationalField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }
};

/**
 * @brief Fluid field, inheriting from EMField for electromagnetic interactions.
 * In our architecture FluidFields are nothing more than more convoluted energy fields, with more complex and specific standing carrier patterns (arising from their
 * standing waves, couplings, nucleus potentials, etc.) and in their case with short-range particle interactions.
 */ 
struct FluidField : public EMField {
    // Energy density distribution. Scalar field representing the total energy concentration per unit volume
    Field<float, 3> energyDensity;

    // Core fluid properties
    Field<float, 3> nucleusPotential;    // Nuclear binding energy + rest mass potential energy (E=mc²). Essential for Solid (body)Physics.
    Field<float, 3> dielectricEnergy;    // Dielectric response energy
    Field<float, 3> shortRangeEnergy;    // Short-range interaction energy

    // Particle-level energy properties. Kinetic energy is essential for solid body physics, in our system
    Field<vec3, 3> kineticEnergy;        // Local, particle-level motion with velocity. Thermal vibrations, pressure-induced motion. Random or chaotic motion.
    Field<vec3, 3> momentumPotential;    // Momentum energy. Coherent, bulk motion of the fluidField. Relates to relativistic mass-energy relationship (E=mc²)
    Field<float, 3> pressureEnergy;      // Compression-expansion energy. Particle-level, converted from kinetic energy
    Field<float, 3> thermalEnergy;       // Particle-level, converted from kinetic energy

    // Optional EM interaction (can be disabled for basic collision)
    bool emInteractionEnabled = false;

    // Medium properties
    bool isCompressible;                 // Gas vs liquid behavior. Compressibility is a property of the medium.
    MediumProperties medium;             // Properties of the medium the field exists in
    float MieScattering;                 // Mie scattering coefficient
    float RayleighScattering;            // Rayleigh scattering coefficient

    // Get effective EM properties based on matter state
    EMFieldProperties getEMProperties() const {
        if (!emInteractionEnabled) return EMFieldProperties::minimal();
        // Return full EM properties considering bound charges, etc
    }

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const EMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }

    applyGravitationalMomentum(const GravitationalField& gravitationalField) {
        // Apply gravitational force to the fluid field momentumPotential
    }

    void propagateAcousticWave(const EnergyTransferEvent& event) {
        // Compute fluid dynamics effects
        float compressionFactor = computeCompression(event);
        float turbulenceFactor = computeTurbulence(event);
        
        // Generate appropriate wave patterns
        if (turbulenceFactor > m_thresholds.turbulenceThreshold) {
            spawnTurbulenceWaves(event.position, turbulenceFactor);
        }
        
        if (compressionFactor > m_thresholds.compressionThreshold) {
            spawnPressureWave(event.position, compressionFactor);
        }
    }
};

// Advanced fluid simulation with detailed energy interactions
struct AdvancedFluidField : public FluidField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;         // Internal friction energy
    Field<float, 3> surfaceTensionEnergy;    // Surface energy
    Field<float, 3> vorticityEnergy;          // Rotational flow energy. Like in turbulence.
    Field<float, 3> turbulenceEnergy;        // Chaotic flow energy. This is the sum of eddyEnergy and dissipationEnergy.
};

// Computational/high-detail fluid simulation with detailed energy interactions
struct ComputationalFluidField : public AdvancedFluidField {
    // Additional computational properties
    Field<float, 3> cavitationEnergy;            // Vacuum bubble energy
    Field<float, 3> boundaryLayerEnergy;         // Detailed wall energy
    Field<float, 3> chemicalEnergy;              // Chemical reaction energy
    Field<float, 3> phaseTransitionEnergy;       // Phase change energy
    Field<float, 3> shockWaveEnergy;             // High-pressure wave energy
    Field<float, 3> acousticEnergy;              // Sound wave energy

    Field<float, 3> bulkModulusEnergy;           // Compression resistance energy
    Field<float, 3> shearModulusEnergy;          // Deformation resistance energy
    Field<float, 3> thermalConductivityEnergy;   // Heat transfer resistance energy

    Field<float, 3> eddyEnergy;                  // Turbulent flow energy
    Field<float, 3> dissipationEnergy;           // Energy loss due to turbulence

    void updateTurbulenceEnergy() {
        // Update turbulenceEnergy, member of AdvancedFluidField, to properly represent the sum of eddyEnergy and dissipationEnergy
    }
};

struct GasField : public FluidField {
    // Add any specific baseline gas properties here
};

// Advanced gas simulation with molecular-level energetics
struct AdvancedGasField : public GasField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;      // Frictional energy
    Field<float, 3> surfaceTensionEnergy; // Surface energy.
    Field<float, 3> vorticityEnergy;      // Rotational flow energy. Like in turbulence.
    Field<float, 3> turbulenceEnergy;     // Chaotic flow energy. This is the sum of eddyEnergy and dissipationEnergy.
    Field<vec3, 3> convectionEnergy;      // Heat flow energy.
    Field<float, 3> diffusionEnergy;      // Mixing energy
};

// Computational/high-detail gas simulation with molecular-level energetics
struct ComputationalGasField : public AdvancedGasField {
    // Additional computational properties
    Field<float, 3> entropyField;                // Disorder energy
    Field<float, 3> chemicalEnergy;              // Chemical reaction energy
    Field<float, 3> phaseTransitionEnergy;       // Phase change energy
    Field<float, 3> shockWaveEnergy;             // High-pressure wave energy
    Field<float, 3> acousticEnergy;              // Sound wave energy

    Field<float, 3> bulkModulusEnergy;           // Compression resistance energy
    Field<float, 3> shearModulusEnergy;          // Deformation resistance energy
    Field<float, 3> thermalConductivityEnergy;   // Heat transfer resistance energy

    Field<float, 3> eddyEnergy;                  // Turbulent flow energy
    Field<float, 3> dissipationEnergy;           // Energy loss due to turbulence

    void updateTurbulenceEnergy() {
        // Update turbulenceEnergy, member of AdvancedGasField, to properly represent the sum of eddyEnergy and dissipationEnergy
    }
};

/**
 * @brief Atmosphere field, inheriting from GasField for gas interactions. Used to model atmospheric conditions.
 * For wind energy, use kinecticEnergy. For temperature, use thermalEnergy from GasField.
 */
struct AtmosphereField : public GasField {
    // Atmosphere-specific properties.
    Field<float, 3> humidityEnergy;         // Atmospheric humidity energy
    Field<float, 3> cloudEnergy;            // Cloud energy
    Field<float, 3> precipitationEnergy;    // Precipitation energy
};

struct AdvancedAtmosphereField : public AtmosphereField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;         // Frictional energy
    Field<float, 3> surfaceTensionEnergy;    // Surface energy. Useful for modeling cloud formation, space reentry skipping for solids, etc.
    Field<float, 3> vorticityEnergy;         // Rotational flow energy. Like in turbulence.
    Field<float, 3> turbulenceEnergy;        // Chaotic flow energy. This is the sum of eddyEnergy and dissipationEnergy.
    Field<vec3, 3> convectionEnergy;         // Heat flow energy.
    Field<float, 3> diffusionEnergy;         // Mixing energy

    // Atmosphere-specific properties
    Field<float, 3> latentThermalEnergy;       // Particle-level, Due to phase transitions of water vapor, among others
};

struct ComputationalAtmosphereField : public AdvancedAtmosphereField {
    // Additional computational properties
    Field<float, 3> entropyField;                // Disorder energy
    Field<float, 3> chemicalEnergy;              // Chemical reaction energy
    Field<float, 3> phaseTransitionEnergy;       // Phase change energy
    Field<float, 3> shockWaveEnergy;             // High-pressure wave energy
    Field<float, 3> acousticEnergy;              // Sound wave energy

    Field<float, 3> bulkModulusEnergy;           // Compression resistance energy
    Field<float, 3> shearModulusEnergy;          // Deformation resistance energy
    Field<float, 3> thermalConductivityEnergy;   // Heat transfer resistance energy

    Field<float, 3> eddyEnergy;                  // Turbulent flow energy
    Field<float, 3> dissipationEnergy;           // Energy loss due to turbulence

    void updateTurbulenceEnergy() {
        // Update turbulenceEnergy, member of AdvancedGasField, to properly represent the sum of eddyEnergy and dissipationEnergy
    }
}

/**
 * @brief Solid field, inheriting from EMField for electromagnetic interactions.
 * In our architecture SolidFields are nothing more than more convoluted energy fields, with more complex and specific standing carrier patterns (arising from their
 * standing waves, couplings, nucleus potentials, etc.) and in their case with long-range particle interactions, in the form of crystalographic lattices.
 */ 
struct SolidField : public EMField {
    // Energy density distribution. Scalar field representing the total energy concentration per unit volume
    Field<float, 3> energyDensity;

    // Core solid properties
    Field<float, 3> nucleusPotential;           // Nuclear binding energy + rest mass potential energy (E=mc²). Essential for Solid (body)Physics.
    Field<float, 3> dielectricEnergy;           // Dielectric response energy
    Field<float, 3> latticeEnergy;              // Crystalographic lattice energy

    // Particle-level energy properties. Kinetic energy is essential for solid body physics, in our system
    Field<vec3, 3> kineticEnergy;               // Particle-level kinetic energy with velocity. In the case of solids, this is the thermal vibrations, pressure-induced motion.
    Field<vec3, 3> momentumPotential;           // Momentum energy. Coherent, bulk motion of the solidField. Relates to relativistic mass-energy relationship (E=mc²)
    Field<float, 3> pressureEnergy;             // Compression-expansion energy. Particle-level, converted from kinetic energy
    Field<float, 3> thermalEnergy;              // Particle-level, converted from kinetic energy

    Field<vec3, 3> strainEnergy;                // Deformation energy, based on Elastic Modulus (cf. Elastic Energy)
    Field<float, 3> elasticEnergy;              // Recoverable deformation energy, based on Elastic Modulus
    Field<float, 3> fractureToughnessEnergy;    // Fracture resistance energy
    Field<float, 3> surfaceTensionEnergy;       // Surface tension energy

    MediumProperties medium;                    // Properties of the medium the field exists in

    // References to EMField for interactions
    FieldCoupling getFieldCoupling(const EMField& other) const {
        // Calculate coupling based on field properties
        return FieldCoupling{...};
    }

    applyGravitationalMomentum(const GravitationalField& gravitationalField) {
        // Apply gravitational force to the fluid field momentumPotential
    }

    void propagateAcousticWave(const EnergyTransferEvent& event) {
        // Compute fluid dynamics effects
        float compressionFactor = computeCompression(event);
        float turbulenceFactor = computeTurbulence(event);
        
        // Generate appropriate wave patterns
        if (turbulenceFactor > m_thresholds.turbulenceThreshold) {
            spawnTurbulenceWaves(event.position, turbulenceFactor);
        }
        
        if (compressionFactor > m_thresholds.compressionThreshold) {
            spawnPressureWave(event.position, compressionFactor);
        }
    }
};

// Advanced solid simulation with crystalline and structural energetics
struct AdvancedSolidField : public SolidField {
    // Additional advanced properties
    Field<float, 3> thermalExpansionEnergy;     // Thermal expansion energy
    Field<float, 3> anisotropicEnergy;          // Directional property energy, based on Elastic Modulus (cf. Elastic Energy)
    Field<float, 3> curvatureEnergy;            // Local surface bending energy
    Field<float, 3> plasticEnergy;              // Permanent deformation energy
    Field<float, 3> crackedEnergy;              // Fracture surface energy
    Field<float, 3> fatigueEnergy;              // Cyclic loading energy
};

// Computational/high-detail solid simulation with crystalline and structural energetics
struct ComputationalSolidField : public AdvancedSolidField {
    // Additional computational properties
    Field<float, 3> dislocationEnergy;          // Crystal defects energy
    Field<float, 3> grainBoundaryEnergy;        // Crystal interface energy
    Field<float, 3> phononEnergy;               // Lattice vibration energy

    Field<float, 3> bulkModulusEnergy;           // Compression resistance energy
    Field<float, 3> thermalConductivityEnergy;   // Heat transfer resistance energy

    Field<vec3, 3> residualStrainEnergy;         // Internal strain energy
    Field<float, 3> shearModulusEnergy;          // Deformation resistance energy
    Field<float, 3> capillaryLengthEnergy;       // Surface tension vs gravity energy
    Field<float, 3> interfacialEnergy;           // Interface energy
    Field<vec3, 3> normalFieldEnergy;            // Surface orientation energy
    Field<float, 3> meniscusHeightEnergy;        // Edge lifting effect energy
    Field<float, 3> spreadingCoeffEnergy;        // Wetting behavior energy
    Field<float, 3> contactAngleEnergy;          // Wetting behavior energy

    // Density emerges from wave amplitude patterns
    struct Density {
        float baseAmplitude;      // Base material density
        float localVariation;     // Density fluctuations
        vec3 gradientFlow;        // Density distribution
        
        float computeDensity(const vec3& point) const {
            float local = m_solver.sampleField(point);
            return baseAmplitude * (1.0f + localVariation * local);
        }
    };
     
    // Elasticity from wave propagation characteristics
    struct Elasticity {
        float waveSpeed;          // Speed of internal waves
        float dampingFactor;      // Energy dissipation rate
        float resonanceFreq;      // Natural frequency
        
        float computeElasticity(const vec3& point) const {
            return waveSpeed * waveSpeed * density.computeDensity(point);
        }
    };

    // Plasticity from permanent wave deformation
    struct Plasticity {
        float yieldThreshold;     // Point of permanent deformation
        float flowRate;           // Rate of plastic deformation
        vec3 strainTensor;        // Directional strain
        
        bool checkYield(float stress) const {
            return stress > yieldThreshold;
        }
    };
};

/**
 * @brief Plasma field, inheriting from EMField for electromagnetic interactions.
 */
struct PlasmaField : public FluidField, public EMField {
    // Plasma properties
    Field<float, 3> ionizationEnergy;         //Ionization energy
};

// Advanced Plasma simulation with molecular-level energetics
struct AdvancedPlasmaField : public PlasmaField, public EMField {
    // Additional advanced properties
    Field<float, 3> viscosityEnergy;      // Frictional energy
    Field<float, 3> surfaceTensionEnergy; // Surface energy.
    Field<float, 3> vorticityEnergy;      // Rotational flow energy. Like in turbulence.
    Field<float, 3> turbulenceEnergy;     // Chaotic flow energy. This is the sum of eddyEnergy and dissipationEnergy.
    Field<vec3, 3> convectionEnergy;      // Heat flow energy.
    Field<float, 3> diffusionEnergy;      // Mixing energy
};

// Computational/high-detail plasma simulation with molecular-level energetics
struct ComputationalPlasmaField : public AdvancedPlasmaField {
    // Additional computational properties
    Field<float, 3> bulkModulusEnergy;           // Compression resistance energy
    Field<float, 3> shearModulusEnergy;          // Deformation resistance energy
    Field<float, 3> thermalConductivityEnergy;   // Heat transfer resistance energy
    Field<float, 3> dissipationEnergy;           // Energy loss due to turbulence
    Field<float, 3> entropyField;                // Disorder energy
    Field<float, 3> chemicalEnergy;              // Chemical reaction energy
    Field<float, 3> phaseTransitionEnergy;       // Phase change energy

    // Eddie currents and turbulence emerge from strong local field coupling.
    // Eddie Currents are loops of electric current induced in a conductor when exposed to a changing magnetic field (Faraday's Law of Induction)
    Field<float, 3> eddyCurrentsEnergy;                  // Turbulent flow energy

    void updateTurbulenceEnergy() {
        // Update turbulenceEnergy, member of AdvancedFluidField, to properly represent the sum of eddyEnergy and dissipationEnergy
    }
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
