/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePhysics.h
 * @brief WavePhysics(c) is a class that handles energy transfer and transformation in the engine.
 * The name may mislead some users into thinking WavePhysics(c) has a very narrow scope, but in fact the choice of the name is intentional, to make clear our
 * paradigm regarding physics. We could have an even more telling name, like WaveFieldPhysics, but we must also had to consider "marketing", appeal, etc.
 * Computer graphics, historically, has not evolved with "immanent" physics properties as a core thing - you can easily notice the signs of this in the fact
 * that most fundamental data structures, like the vertex, were never designed holding physics attributes in them. This is a testament to the disproportionately
 * visually oriented paradigm that evolved in the industry, in the efforts to model realistic worlds - in fact, a fundamental deficiency of the evolution of its
 * design vision.
 * So, keep this in mind, while you follow our design and implementation. That we try our best to work around this fact, trying to "crawl" into the core
 * data structures, since the Vulkan core, to gain a better place in the sun for Physics. Not like a "fragile" messed mix of short-sighted fixes, but as a
 * more structured and "feeling automatic", "for free" design and more tightly coupled with the core player experience, more coherent, more consistent, more 
 * elegant whenever possible also.
 * 
 * ARCHITECTURAL NOTES:
 * - We're fundamentally treating everything through an energy-based approach:
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
 *      - Fields, along with Waves, are the core building blocks in WavePhysics. and they're basically always representing energy distributions in space, be 
 *       it potential or converted. So, basically, at the heart of our architecture we have:
 * 
 *          Energy (the quantity/data) --> Wave (the carrier pattern: excitation/perturbation/radiation in a field) --> Field (the set/system: energy distribution in space)
 * 
 * - Main elements, entities in the WavePhysics system:
 *          - Field: Energy distribution in space, be it potential or converted: kinetic/momentum, etc.
 *          - Dimensional/Spectral Energy: Energy represented as a 4D vector (spectral components), which we can project to scalar when needed for simple calculations, 
 *          rich interaction possibilities between components.
 *          - Wave: Energy excitation/perturbation/radiation in a field.
 *          - Wavefront: Wave propagation path. In general we are referring to the Wavefront Tracker: the point in the field where the excitation value of the 
 *          wave/continuum crossed the limiar threshold - manifesting energy enough to be considered a wavefront.
 *          - Short-lived Particle/Wavefront Tracker: A computational marker for wavefront tracking. In general, referring to "short-lived waves" - waves whose
 *          dissipation is so fast/short lived that it's not worth tracking them any longer. They're meant to contribute to intense and short-lived energy 
 *          transfer in phenomena interesting for realtime games, like impacts, explosions, combustions, heating, transfer of kinectic energy, sound
 *          generated by fluid compression/push, shockwaves, luminous phenomena, ionization (plasma filaments, fields), in various degrees of intensity and 
 *          wavefront sampling density. In the technical side, what we model them to go through is actually physics events like particle decay, pair production, 
 *          change of energy state (energy signature, like "it's a quark", "a photon", "a boson"), change of atomic nuclei/signature, etc.
 *          - Energy Manifestation: The way energy is expressed in the system. It can be kinetic, thermal, particle, wave (radiation, electromagnetic plus other
 *          known and support for custom/exoteric - 'exoteric' in the sense of the frontier of physics, extreme physics, astrophysics), field.
 *          - Impact/Collision: Energy transfer between objects. We use these names for easy recognition by users, familiarity, but conceptually we see these
 *          events much more like energy transfer events and also as some sort of temporary entanglement/ressonance/composition, like we see in standing waves, 
 *          wave superposition/composition, quantum physics: their phases interact, ressonate, modulate one another, based on their characteristics.
 *          - So every time you see the word "impact" or "collision" in the code, it implies actually a wave interference event involving energy transfer.
 *          Like energy rays crossing each other after being emitted: the point where they intertwine will manifest the result of their interaction, in general
 *          manifesting added energy, change of energy state, energy level in that region - like solving their wave-like superposition. It can go from microwave
 *          ressonator devices for game weapons to solving of car crashes, explosions, crumbling of buildings, shock between astrophysical bodies, quasars, etc.
 *          - Dimension: a set of values seem as  whole. The most defined, "stable", sampled stretches of them can be also seen as states - like in "the states 
 *          of matter", or in particle categories (neutrino, photon, boson, etc).
 *          - Local Domain, Global Domain: We separate the domain/context of the system into two main categories.
 *          The Local Domain is used for further detailed, fine-grained solving. Also in scenarios where we may want to "isolate"/contain a system and its internal
 *          dynamics for the sake of making it possible to simulate such a system in realtime, performance-wise - electromagnetic fields, plasma-like fields,
 *          including vast ones like nebulas, but also very practical ones for games and other apps, like a battle arena sector (so sectors can be toggled on/off), 
 *          including also, possibly, exotic physics scenarios/systems - like the horizon of a black hole, etc. This means, naturally, that we may want to set
 *          some or all physics rules/laws as custom ones, to be different in a given local domain. Think of the various local domains as the branches in a tree,
 *          nodes in a node graph - in fact, this system will be designed analogous to the Scene Graph paradigm. A Local Domain would then be akin to a Layer in
 *          a usd scene composition. And this also means we leverage this granular, branching, already fractal structure to selectively load/unload and update 
 *          domains as needed, with LOD strategies, streaming, etc.
 *          The Global Domain is where we can have more general and pervasive, simple, straightforward physics, like general gravity, management of and state
 *          keeping, sync, of collisions, etc.
 *          Bonds, Links: we try to model and simplify otherwise complex and ultra-detailed physics interactions, from large scale systems to very small ones
 *          (specially useful for these last ones) using the concept of bonds and links. Strong stable electromagnetic bonds, for instance, or the energy involved,
 *          in "potential form", not released, required to break a stable bond. Matter, solid volumes are some obvious examples. Then shattering, destruction,
 *          fracturing become events which involve and require imense amounts of energy, releasing proportional amounts of energy, in various manifestation types,
 *          until all that energy is spent, finally dissipated and reabsorbed by the environment/system. That means, naturally, automatically, that the most
 *          still, static scenarios are the ones that require the least amount of processing but at the same time the ones which ironically are the most energy
 *          intense - imense amounts of energy is involved to keep them so static, and all that energy is there, waiting to be released.
 *          - Physics Thread/Physics Update: the physics solving, the solving of so many complex and dynamic elements mean we need to have a dedicated thread,
 *          also leverage multi-threading in it (with proper sync objects, of course). From the perspective of the Physics engine, subsystem, it is indeed the
 *          most fundamental core structure in the engine. All interactions emerge from its existence - the less hardcoded and scripted, the more this is true.
 *          It's as if the gameplay thread is mostly "fake" (in the sense of reflecting the real world), scripted, non-spontaneous: it's most like a "force of will"
 *          factor, a wishlist. While the physics thread and the physics system are much more complex and nuanced, dynamic and emergent. What prevents us from
 *          letting it spread more in realtime is of course performance constraints. And in Hydragon we have the proper harmonic mechanisms to gracefully lead
 *          the possible emergent scenarios to fruitful, interesting ones - e.g. the PatternOrchestrator.
 *          - *Regarding traditional particle emitters: in WavePhysics, to keep with Energy Conservation, etc, we probably won't introduce emitters, if possible,
 *          as we're trying to stick to a "makes sense", phenomenological approach, considering Energy Conservation Law, Cause-Effect, etc. So, to model an emitter
 *          (arbitrary emission event) we resort to adding a field plus adding "fuel" (energy) to a region of the field. If a proper field is already present we
 *          just add energy to it. This can be useful, for instance, to model magic fx. For more realistic effects, try to simplify cause-effect, energy
 *          conservation, energy transfer, etc. e.g. for a fire effect for a torch, we can sample/query a pre-set thermalEnergy value in the torch prop, adding
 *          also, automatically, a local field, for more resolution.
 * 
 * - Spatial Partitioning: Grid-based division for parallel compute, Local space calculations, Neighbor search optimization. Adaptive granular refinement of these
 * grids, for further optimization (they can go from very sparse to very dense, depending on the needs of the simulation). Irregular grids for complex and exotic 
 * shapes, dimensional geometries.
 * - Selective Physics: Priority-based simulation. LOD for physics calculations, Distance-based activation.
 * - Energy Conservation: Track energy states, Optimize transfer calculations, Handle dissipation efficiently.
 * - Energy Manifestation: State transitions, Conservation rules, Dissipation patterns.
 * - Constraint Solving: Parallel constraint resolution, Iterative refinement, Stability maintenance.
 * - Coherent Memory Patterns: Buffer compaction, Cache optimization, Memory alignment.
 * - Compute Scheduling: Load balancing, Work distribution, Dependency management.
 * - Hybrid approach: Use Vulkan for real-time deformation, Taichi for complex scenarios, Blend between approaches based on needs.
 * - Efficient parallel processing, Selective physics simulation, Visual accuracy over physical accuracy, Scalable performance, High-quality results.
 * - Wave-Based Modeling: Uses wave functions as primary representation, Converts to particles only when needed, Maintains energy conservation.
 * - Adaptive precision using exponential encoding, Dimension reduction where possible, Local space calculations, Wavelet compression.
 * - Smart Memory Management: Compact struct layouts, Bit-field usage for quantized values, Adaptive grid refinement.
 * - Physics Approximation: Energy-based approach instead of direct forces, Wave propagation for continuous effects, Local space optimization.
 * - Energy Transfer System: Energy is never lost, only transformed, Automatic spawning of appropriate reactions based on thresholds, Different manifestation 
 * types (kinetic, thermal, particles, etc).
 * - Adaptive Precision: Dynamic bit depth based on energy magnitude, Higher precision for high-energy regions, Memory optimization for low-energy areas.
 * - Reaction System: Automatic effect spawning based on energy thresholds, Configurable reaction templates, Scale effects based on energy magnitude.
 * - Optimization Features: Progressive quantization levels, Wavelet compression with variable precision, Efficient memory usage for different energy manifestations.
 * - Sparse Particle System: Optimized storage with bit-packed flags, Distance-based culling, Minimum energy thresholds for early termination.
 * - Energy Propagation: Sphere of influence calculation, Energy transfer through space, Customizable falloff curves, Wave/field contribution integration.
 * - Multi-sampling System: Octave generation for detail enhancement, Energy scaling per generation: Controlled spawning based on importance. 
 * - Trajectory System: Multiple path types (linear, ballistic, spiral), Field-guided motion, Collision energy transfer.
 * - Optimization Features: Sparse sampling by default, Optional multi-sampling for important events, Early culling of low-energy particles, Packed data structures.
 * - Wave-Centric Approach: Uses "particles" merely as computational markers for wavefront tracking, Focuses on wave properties (amplitude, frequency, phase) rather 
 * than particle properties, Handles proper wave behavior (interference, superposition, attenuation).
 * - Field Integration: Treats each "particle" as a wave contribution point, Computes field strength through wave superposition, Handles medium transitions and 
 * field interactions.
 * - Energy Propagation: Models energy transfer as wave propagation, Accounts for medium properties (attenuation, dispersion), Handles wave-field interactions.
 * - Optimization: Uses short-lived markers to track wave peaks efficiently, Adapts precision based on energy levels, Maintains the sparse sampling approach while 
 * modeling continuous phenomena.
 * - Frequency Bands: Models the entire electromagnetic spectrum, Maps frequencies to physical effects, Includes thresholds for different interaction types.
 * - Interaction Effects: Ionization potential, Mutation probability, Decay chain reactions, Material penetration depth.
 * - Chain Reactions: Higher frequencies can trigger particle decay, Secondary radiation waves, Probability-based chain reactions.
 * - This naturally models phenomena like: Radiation shielding (penetration depth), Nuclear decay chains, Ionizing vs non-ionizing radiation, Quantum effects at 
 * high energies.
 * - Dimensional Energy: Energy represented as a 4D vector (spectral components), Can project to scalar when needed for simple calculations, Rich interaction 
 * possibilities between components.
 * - Modulation Field: 4D noise field (3D space + energy dimension), Warping based on energy levels, Multi-octave sampling for different scales, Cached regions 
 * for optimization.
 * - Wave-Field Interaction: Non-linear modulation effects, Energy-dependent wave behavior, Quantum effects at high energy levels, Propagation through energy landscape.
 * - Optimization Features: Cached sampling for frequently accessed regions, Adaptive step size based on energy levels, Scalar projection for simple calculations. 
 * - This creates interesting emergent behaviors: Waves can "resonate" with certain energy levels in the field, High-energy waves can "tunnel" through low-energy regions, 
 * Field modulation can create localized high-energy phenomena, natural emergence of quantum-like behaviors at high energies.
 * - Domain Management Strategies: Streaming Domains: Like the chunks in OpenWorldExample.cpp, but for physics. Priority-based Processing: Similar to StreamingControlState
 * Dynamic LOD System: Based on WaveletMesh.h's approach.
 * - Edge Case Opportunities: High-energy concentrations could create temporary "anomalies" (gameplay features), Solver boundary interactions might generate interesting 
 * interference patterns, numerical "instabilities" could be channeled into visual effects.
 * - There's a lot more even more interesting notes about the WavePhysics system and node in Source/Core/Physics/WavePhysicsNode_README.md.
 * 
 * Physics System Comparison:       Runtime Memory, MB	                    VRAM Usage, MB	            Binary Size, MB	            Relative Performance
 * 
 * Bullet Physics	                50-150	                                N/A	                        2-5	                        1.0x (baseline)
 * PhysX	                        100-300	                                50-150	                    8-15	                    1.2x
 * Havok	                        80-200                                  N/A	                        5-10	                    1.1x
 * WavePhysics	                    15-60                                   10-40                       3-6                         2.0-3.0x
 * 
 * Latest (theoretical) improvements shows:
 *
 * ~50% reduction in runtime memory usage through batch processing and compression
 * ~50% reduction in VRAM usage through field grid optimization
 * No significant change in binary size
 * ~30% performance improvement due to SIMD-friendly batch operations and better cache utilization
 * 
 * High Activity Zone │ Medium Activity       │ Low Activity Zone
 * ───────────────────┼───────────────────────┼───────────────────
 * 32-bit precision   │ 16-bit                │ 8-bit
 * Full update rate   │ 1/2 rate              │ 1/4 rate
 * All effects        │ Major effects         │ Essential only
 * Cached results     │ Partial cache         │ Fully cached
 * 
 * Active Domains     │ Ready Queue           │ Dormant
 * ───────────────────┼───────────────────────┼────────────
 * Full simulation    │ Physics state cached  │ Serialized
 * Real-time updates  │ Quick activation      │ Disk stored
 * Memory resident    │ Memory mapped         │ Compressed

 * Level 1: Local optimizations (per solver)
 * ↓
 * Level 2: Domain-level caching & streaming
 * ↓
 * Level 3: Global system management
 * ↓
 * Level 4: Emergency fallbacks
 * 
 * WAVE PHYSICS MEMORY ANALYSIS

Current Wave Size: ~48 bytes
Optimized Wave Size: ~24 bytes
Current Field Cell: ~320 bytes
Optimized Field Cell: ~64 bytes

DELTA ENCODING SCHEME

Base Values (Priority Zone):
- Position: 32-bit float (xyz)
- Energy: 32-bit float
- Velocity: 32-bit float (xyz)
- Properties: 32-bit packed

Delta Encoding Tiers (similar to AdaptiveMesh's DeltaCompressionScheme):

1. High-Energy Deltas (Level 1):
    struct Level1Delta {
        uint32_t cellIndex : 24;    // Up to 16M cells
        uint8_t deltaE : 4;         // Energy delta
        uint8_t deltaV : 4;         // Velocity magnitude delta
    }; // 4 bytes total

2. Mid-Energy Deltas (Level 2):
    struct Level2Delta {
        uint32_t cellIndex : 24;
        uint8_t deltaE : 3;
        uint8_t deltaV : 3;
        uint8_t flags : 2;
    }; // 4 bytes total

3. Low-Energy Deltas (Level 3+):
    struct Level3Delta {
        uint32_t cellIndex : 24;
        uint8_t deltaE : 2;
        uint8_t deltaV : 2;
        uint8_t flags : 4;
    }; // 4 bytes total

Storage Optimization:
- High activity: Full values + Level1 deltas
- Medium activity: Base values + Level2 deltas
- Low activity: Minimal base + Level3 deltas
- Dormant: Delta-only or disabled

WAVE PHYSICS MEMORY ANALYSIS (OPTIMIZED WITH SELECTIVE FIELDS, STRUCT LOD & DELTA ENCODING, ADAPTIVE GRID & SPARSE STORAGE)

Struct Sizes (Priority Zone):
- Full Wave Info: ~24 bytes
- Full Cell Info: ~64 bytes (base mechanical)

Struct Sizes (Non-Priority):
- Simplified Wave: ~8 bytes (essential properties + deltas)
- Simplified Cell: ~16 bytes (core state + deltas)

Delta Encoding (similar to AdaptiveMesh's approach):
- High Precision: 32-bit base + 8-bit deltas
- Medium Precision: 16-bit base + 4-bit deltas
- Low Precision: 8-bit base + 2-bit deltas

SIMD Batch Processing:
- Wave Batches: 256 waves per compute dispatch
- Cell Batches: 64x64x64 cell blocks
- Memory Layout: AoS for CPU, SoA for GPU

|--------------------|--------------|---------------|---------------|--------------|----------------|
| Scale Tier         | Grid Size    | Active Waves  | Dense Memory  | Optimized    | Comparable To  |
|--------------------|--------------|---------------|---------------|--------------|----------------|
| Small Scene        | 16³ (4096)   | 100           | 1.3 MB        | 30-40 KB     | Small VFX      |
| (4096 cells)       |              |               | (1.28 + 0.05) | (25 + 5)     |                |
|--------------------|--------------|---------------|---------------|--------------|----------------|
| Medium Scene       | 32³ (32768)  | 1000          | 10.5 MB       | 200-300 KB   | Typical Game   |
| (32768 cells)      |              |               | (10.24 + 0.24)| (175 + 25)   | Physics Scene  |
|--------------------|--------------|---------------|---------------|--------------|----------------|
| Large Scene        | 64³ (262144) | 5000          | 84 MB         | 1-1.5 MB     | Open World     |
| (262144 cells)     |              |               | (83.9 + 0.12) | (0.9 + 0.1)  | Section        |
|--------------------|--------------|---------------|---------------|--------------|----------------|
| Huge Scene         | 128³ (2.1M)  | 20000         | 671 MB        | 6-8 MB       | Full Open      |
| (2.1M cells)       |              |               | (671 + 0.9)   | (5.5 + 0.5)  | World Area     |
|--------------------|--------------|---------------|---------------|--------------|----------------|
| Extreme            | 256³ (16.7M) | 100000        | 5.4 GB        | 40-50 MB     | Large Scale    |
| (16.7M cells)      |              |               | (5.36 + 0.04) | (37.5 + 2.5) | Simulation     |
|--------------------|--------------|---------------|---------------|--------------|----------------|

Priority Zone Management:                                       Delta Optimization:                                     Adaptive Grid Resolution:
    - Inner Zone (r < 32m): Full structs, 32-bit precision          - Store base values at lower precision                - High resolution in dense areas- High activity: Full resolution
    - Mid Zone (32m-128m): Simplified structs, 16-bit + deltas      - Use relative deltas between adjacent cells          - Medium: 2x2x2 merged cells
    - Outer Zone (>128m): Minimal structs, 8-bit + deltas           - Pack multiple deltas into single bytes              - Low: 4x4x4 merged cells
    - Dormant: Delta-only or disabled                               - Progressive precision reduction with distance       - Result: 50-75% reduction in cell count

Sparse Storage:                                     Compute Shader Strategy:                        Memory Layout:
   - Only store active cells                            - Priority cells: Processed every frame         - GPU: SoA for SIMD efficiency
   - Hash-based lookup for sparse regions               - Mid-priority: Every 2-4 frames                - CPU: AoS for cache coherency
   - Occupancy maps for quick traversal                 - Low-priority: Every 8-16 frames               - Shared: Double-buffered for async updates
                                                        - GPU occupancy-based CPU fallback

Additional Savings:                                 Combined Impact:
    - Delta encoding: 40-60% reduction                  - Dense areas: Full resolution + delta encoding
    - Struct LOD: 50-70% reduction                      - Sparse areas: Reduced resolution + minimal storage
    - SIMD batching: 20-30% processing speedup          - Empty areas: No storage (occupancy map only)
    - Compute offload: 60-80% CPU load reduction        - Overall: 80-90% reduction from worst case

(Memory shown as: (Grid Memory + Wave Memory)                                           COMPRESSION POTENTIAL:
                                                                                            - Small/Medium scenes: Minimal compression needed
    - Selective field enabling reduces base memory by 60-80%                                - Large scenes: 50-70% reduction with adaptive resolution               
    - LOD system provides additional 40-60% reduction                                       - Huge/Extreme: Required aggressive optimization + streaming
    - Adaptive precision (bit depth, etc) saves 30-50% in medium/low activity zones
    - Adaptive resolution could reduce grid memory by 40-80%
    - Sparse storage could save additional 30-60% in empty regions
    - Dormant regions consume negligible memory
    
REAL-TIME CONSTRAINTS:
    - Small/Medium: Can run at full precision
    - Large: May need LOD system
    - Huge/Extreme: Requires streaming and aggressive optimization

 * TODO:
 * - Cleanup and organize the functionality and structs we introduced in the latest task session.
 *      - Replace many or all of the structs definitions inside of OptimizationSystem struct with just assigment of variables from existing structs. e.g. 
 *      FluidField settings already exist in its definition, so there's no need to create redundant structs there. It was a redundancy mistake introduced 
 *      by the assistant. 
 *      - Consider which classes other than WavePhysics should be moved to dedicated files.
 *      - Organize the existing code into logical sections and functions.
 * - Check thoroughly, including with assistant, to eliminate redundancies, inconsistencies in the content, even it being in design sketch phase. A quick way to
 * do this with the assistant is: 1) ask him to read and summarize the file 2) then, ask about inconsistencies, redundant code, etc.
 * - We could move some of the content here related to setup of solid body to Scene.h, but by design we want to maintain separation of concerns and also try, to
 * some degree, to keep the workflow for users parallel to using ECS - keeping specific functionality in components, adding components, etc. So we do the same but
 * using nodes. So, the user will create and connect a BoxFieldNode pattern (as a user in Unity or Unreal would add a Collider Component), and then connect it to a 
 * WavePhysics.
 * - Create the WavePhysics_README.md in the same folder as this file.
 * - We'll start from the core conceptual elements, data structures and dynamics (fields, potential wells, energy conservation principles, wave-field interaction,
 * excitation dynamics), but then later we'll gradually incorporated layers of physics features towards the more superficial layers - like rigid body dynamics,
 * character, environment, etc. In that moment, for instance for rigid bodies, we may incorporate Bullet Physics, if we deem appropriate enough.
 * - Complete WavePhysicsSolver implementation
 * - Implement Vulkan-style configuration system
 * - Add methods for inter-solver communication
 * - Implement quantum effects handling
 * - Decide if we're going to create the Physics submodule from scratch, using Compute, etc, or if we're going to use a third-party physics library, like Taichi.
 * Or if we're going to use both, toggling at will, in a hybrid approach.
 * The idea of first concentrating on the core design of the physics system is not only the obvious way to do it but also to approach the physics side of the engine in a
 * way that hopefully it will lead to emergent physics behaviors, interactions - less work for the user and for us, more marveling at spontaneous phenomena, etc. We'll
 * profit from the fact that we don't have to prove anything or beat competitors: there's already a lot of good physics engines out there, and we're not specialists in
 * this field also. It will be more of a rich experimentation and discovery process, and we'll be able to learn a lot from it.
 * - The implementation is meant to be lightweight, efficient, and modular enough so that even WavePhysics being used as a singleton, adding mmany local solvers don't
 * introduce a significant overhead to the whole system. General practice will be to have a higher resolution solver/sytem, more global. And then instantiate other ones 
 * for local and very optimized, quantized effects, and rename these properly, like "GlobalWavePhysics", "CharacterWavePhysics", "Battleground01WavePhysics", etc.
 * 
 * Phase 1: Basic System
* - Implement core wave propagation
* - Basic domain management
* - Simple thresholds

* Phase 2: Optimization Layer
* - Add streaming system
* - Implement LOD system
* - Setup caching

* Phase 3: Edge Case Handling
* - Dynamic thresholds
* - Anomaly detection
* - Effect generation

* Phase 4: Experimentation
* - Test extreme scenarios
* - Document emergent behaviors
* - Create gameplay features
* 
* Energy Density   → Adjust precision
* Activity Level   → Manage update rates
* Memory Usage     → Control streaming
* Solver Load      → Balance computations
* Edge Behaviors   → Generate effects
*/
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "ShortLivedParticle.h"
#include "Node.h"
#include "PhysicsFields.h"

namespace hd {

struct EnergyTransferInfo {
    // Progressive quantization levels
    struct Quantization {
        uint8_t highEnergy : 8;    // Full precision for critical areas
        uint8_t mediumEnergy : 6;  // Reduced for medium intensity
        uint8_t lowEnergy : 4;     // Minimal for background effects
    };

    // Energy transfer rules
    struct TransferRule {
        EnergyManifestationType::Type sourceType;
        EnergyManifestationType::Type targetType;
        float threshold;
        float conversionEfficiency;
        
        // Automatic reaction spawning
        struct Reaction {
            std::string effectTemplate;  // "explosion", "debris", "spark"
            float intensityScale;        // Scale effect by energy magnitude
            uint32_t particleCount;      // Dynamic particle count
            float lifetime;              // Effect duration
        } reaction;
    };
};

// Energy manifestation types and thresholds
struct EnergyManifestationType {
    enum class Type : uint8_t {
        KINETIC,        // Motion, velocity
        THERMAL,        // Heat, temperature
        DEFORMATION,    // Shape changes
        PARTICLE,       // Break-up, debris
        WAVE,          // Shock waves, ripples
        FIELD          // Electromagnetic, force fields
    };

    // Energy bands based on frequency ranges
    struct FrequencyBand {
        static constexpr float kRadioWaves     = 1e3f;    // Low energy EM
        static constexpr float kInfrared       = 1e12f;   // Thermal radiation
        static constexpr float kVisible        = 1e15f;   // Visible light
        static constexpr float kUltraviolet    = 1e16f;   // Starting to be harmful
        static constexpr float kXRay          = 1e18f;   // Ionizing radiation
        static constexpr float kGammaRay      = 1e20f;   // Highly penetrating
        static constexpr float kCosmicRay     = 1e24f;   // Ultra-high energy
    };

    // Interaction thresholds
    struct InteractionThreshold {
        static constexpr float kMolecularBond  = 1e-19f;  // Can break molecular bonds
        static constexpr float kIonization     = 1e-17f;  // Can ionize atoms
        static constexpr float kNuclearEffect  = 1e-13f;  // Can affect nuclear structure
        static constexpr float kParticleDecay  = 1e-10f;  // Can induce particle decay
    };
};

class SolidField {
    // Solid body properties
    Field<float, 3> density;         // Mass density
    Field<vec3, 3> velocity;         // Velocity field
    Field<float, 3> pressure;        // Pressure field
    Field<vec3, 3> stress;           // Stress tensor field
};

class WavePhysics : public Node {
public:
    // === Structure Definitions ===
    // Physics properties and states are represented as field interactions
    struct PhysicsField {
        // Field properties that affect solid bodies
        struct SolidField {
            float energyDensity;      // Relates to mass and momentum
            float fieldStiffness;     // Controls elasticity behavior
            float waveImpedance;      // Material resistance to wave propagation
            vec3 momentumField;       // Directional energy flow
        };
    };

    // TODO: In fact, this field type, FluidField and others possibly could derive from a base Field struct,
    // for they already share many properties.
    struct ElectromagneticField {
    
    
        // Interface behaviors (similar to fluid interfaces)
        struct BoundaryBehavior {
            float surfaceCharge;     // Like surface tension
            vec3 boundaryNormal;     // Similar to fluid surface normal
            float skinDepth;         // EM penetration (cf. fluid penetration)
            
            struct Discontinuity {
                float impedance;     // Like fluid impedance
                float reflection;    // Field reflection coefficient
                float refraction;    // Field bending at interfaces
            };
        };
    
        // Unified field effects (shared with fluid dynamics)
        struct FieldEffects {
            struct Forces {
                vec3 lorentzForce;   // EM force on charges
                vec3 magneticPressure; // Like fluid pressure
                vec3 radiation;      // EM energy emission
            };
    
            struct Instabilities {
                float kinkInstability;   // Like K-H instability
                float tearingMode;       // Field line breaking
                float plasmoid;          // Bubble-like structure
            };
        };
    
        // Conservation laws (parallel to fluid conservation)
        struct Conservation {
            bool chargeConserved;    // Like mass conservation
            bool fluxConserved;      // Like momentum conservation
            float energyBalance;     // Energy conservation
            
            struct Constraints {
                float divB;          // No magnetic monopoles
                float divE;          // Gauss's law
                vec3 curlE;         // Faraday's law
                vec3 curlB;         // Ampère's law
            };
        };
    };


    // Field properties that affect surface tension - border layer between fluids
    struct SurfaceField {
        float surfaceTension;        // Surface energy density
        float contactAngle;          // Fluid-surface interaction
        vec3 normalField;           // Surface orientation
        float curvature;            // Local surface bending
        float interfacialEnergy;    // Between different fluids
        
        struct Capillarity {
            float capillaryLength;   // Surface tension vs gravity
            float meniscusHeight;    // Edge lifting effect
            float spreadingCoeff;    // Wetting behavior
        };
    };

    // Field properties that affect phase transitions
    struct PhaseField {
        float phaseFraction;        // Relative amount of each phase
        float interfacialArea;      // Contact area between phases
        float phaseTransitionRate;  // Phase change speed
        float latentHeat;          // Energy for phase change
        
        struct MixingProperties {
            float miscibility;      // How easily phases mix
            float diffusivity;      // Mass transfer rate
            float schmidtNumber;    // Momentum vs mass diffusion
        };
    };

    // Field properties that affect (energy) transport processes
    struct TransportField {
        float thermalConductivity;  // Heat transfer ability
        float diffusionCoeff;      // Mass transfer rate
        float prandtlNumber;       // Momentum vs heat diffusion
        float pecletNumber;        // Advection vs diffusion
        
        struct ConvectionProperties {
            float rayleighNumber;   // Buoyancy-driven flow
            float nusseltNumber;    // Convective heat transfer
            float grashofNumber;    // Buoyancy vs viscosity
        };
    };

    // Field properties that affect stability and instabilities
    struct StabilityField {
        float weberNumber;         // Surface tension vs inertia
        float froideNumber;        // Inertia vs gravity
        float machNumber;          // Flow speed vs sound speed
        float cavitationNumber;    // Pressure vs vaporization
        
        struct InstabilityModes {
            float kelvinHelmholtz;  // Shear instability
            float rayleighTaylor;   // Density instability
            float plateauRayleigh;  // Surface tension breakup
        };
    };

    // Field patterns that emerge into familiar physics behaviors
    struct EmergentBehaviors {
        // Mass emerges from energy density patterns in the field
        struct MassPattern {
            float localEnergyDensity;    // Core mass property
            float inertialResistance;    // Resistance to field changes
            vec3 energyGradient;         // Direction of mass distribution
        };

        // Solid body cohesion emerges from strong local field coupling
        struct CohesionPattern {
            float bondStrength;          // Strong field coupling = rigid
            float internalResonance;     // Natural frequency of the material
            float structuralIntegrity;   // Overall field stability
        };

        // Motion emerges from wave propagation patterns
        struct MotionPattern {
            vec3 momentumWave;           // Directional energy flow
            float phaseVelocity;         // Wave propagation speed
            float groupVelocity;         // Energy propagation speed
        };
    };

    struct WaveProperty {
        float amplitude;
        float frequency;
        float phase;
        vec3 propagationDirection;
        
        // Wave characteristics
        struct MediumInteraction {
            float wavelength;
            float attenuation;
            float dispersion;
            float penetration;      // Material penetration depth
            float ionizationRate;   // Rate of ionization effects
        } medium;
        
        // Effect properties based on frequency
        struct Effects {
            bool canIonize;
            bool canInduceMutation;
            bool canTriggerDecay;
            float chainReactionProbability;
        } effects;

        // Compute interaction effects based on frequency
        Effects calculateEffects() const {
            Effects e;
            e.canIonize = frequency >= FrequencyBand::kUltraviolet;
            e.canInduceMutation = frequency >= FrequencyBand::kXRay;
            e.canTriggerDecay = frequency >= FrequencyBand::kGammaRay;
            e.chainReactionProbability = 
                frequency >= FrequencyBand::kCosmicRay ? 
                    (frequency / FrequencyBand::kCosmicRay) : 0.0f;
            return e;
        }

        // Get penetration depth based on frequency
        float calculatePenetrationDepth(float materialDensity) const {
            // Higher frequencies penetrate deeper
            float basePenetration = std::log10(frequency) * 0.1f;
            return basePenetration / materialDensity;
        }
        
        // Dynamic precision based on amplitude
        uint32_t getBitDepth() const {
            if (amplitude > EnergyManifestationType::kHighEnergyThreshold) return 32;
            if (amplitude > EnergyManifestationType::kMediumEnergyThreshold) return 16;
            return 8;
        }
    };

    // Material properties and states emerge from wave behavior patterns in the field
    struct MaterialField {
        struct PropertyFields {
            // Density emerges from wave amplitude patterns
            struct DensityField {
                float baseAmplitude;      // Base material density
                float localVariation;     // Density fluctuations
                vec3 gradientFlow;        // Density distribution
                
                float computeDensity(const vec3& point) const {
                    float local = m_solver.sampleField(point);
                    return baseAmplitude * (1.0f + localVariation * local);
                }
            } density;

            // Elasticity from wave propagation characteristics
            struct ElasticityField {
                float waveSpeed;          // Speed of internal waves
                float dampingFactor;      // Energy dissipation rate
                float resonanceFreq;      // Natural frequency
                
                float computeElasticity(const vec3& point) const {
                    return waveSpeed * waveSpeed * density.computeDensity(point);
                }
            } elasticity;

            // Plasticity from permanent wave deformation
            struct PlasticityField {
                float yieldThreshold;     // Point of permanent deformation
                float flowRate;           // Rate of plastic deformation
                vec3 strainTensor;        // Directional strain
                
                bool checkYield(float stress) const {
                    return stress > yieldThreshold;
                }
            } plasticity;
        } properties;

        /**
         * @brief Dynamic material state tracking, including fields describing the state of matter in electromagnetic contexts. 
         *  
         * This is a complex data structure that encompasses many different fields and properties,
         * each representing a different aspect of the material's state. It's designed to be flexible
         * and adaptable to a wide range of material types and scenarios.
         * TODO: 
         * - Cleanup, organize, tighten up the code, for better readability and performance.
         * - Add any missing essential functions.
         * - Almost certainly optimize, get rid of some structs, properties, for better performance, specially
         * after practical testing.
         */
        struct StateFields {
            // Energy storage and dissipation, including electromagnetic and mechanical energy
            struct EnergyStorage {
                // Stored Electromagnetic energy, including Potential energy from resisting EM field topology
                float topologicalPotential;   // Energy stored by maintaining position
                float orientationalEnergy;    // Energy from alignment/misalignment
                float domainWallEnergy;       // Energy stored in magnetic domains
                
                struct ResistiveEffects {
                    float hysteresis;         // Energy lost/stored in field cycling
                    float eddyCurrentLoss;    // Energy dissipated in current loops
                    float magneticDamping;    // Energy dissipation rate
                };
                ResistiveEffects resistiveEffects;

                // Stored mechanical energy
                float mechanicalPotential;  // Stored mechanical energy

                float EMPotential() const {
                    return topologicalPotential + orientationalEnergy + domainWallEnergy;
                }

                float totalPotential() const {
                    return EMPotential() + mechanicalPotential;
                }

                float EMResistiveLoss() const {
                    return resistiveEffects.hysteresis + resistiveEffects.eddyCurrentLoss;
                }
            } energyStorage;

            // Stress field representation
            struct StressField {
                vec3 principal;           // Principal stress directions
                float magnitude;          // Stress intensity
                mat3 tensor;              // Full stress tensor, including stress from field resistance
                float strain;             // Deformation due to field forces
                float potential;          // Stored energy from field opposition

                struct MagneticStress {
                    float magnetostriction;   // Field-induced mechanical stress
                    float magneticAnisotropy; // Directional resistance
                    float domainAlignment;    // Internal magnetic structure
                };

                struct ElectricStress {
                    float dielectricConstant; // Material's dielectric properties
                    float polarization;       // Internal electric field alignment
                    float capacitance;        // Energy storage capacity
                };

                struct ThermalStress {
                    float thermalExpansion;   // Material's thermal expansion properties
                    float thermalConductivity; // Heat transfer rate
                    float thermalCapacity;    // Energy storage capacity
                };

                struct MechanicalStress {
                    float elasticModulus;     // Elasticity of the material
                    float yieldStrength;      // Stress at which plastic deformation begins
                    float ultimateStrength;   // Maximum stress the material can withstand
                    float fractureToughness;  // Material's resistance to fracture
                };

                // Gravitational stress is only relevant in scenarios where gravity is significant, like planetary bodies.
                struct GravitationalStress {
                    float gravitationalForce; // Gravitational force
                    float gravitationalPotential; // Gravitational potential energy
                    float gravitationalDamping; // Gravitational dissipation rate
                };

                // Only ever useful in scenarios where quantum effects are significant, like
                // in extreme conditions (high energy particles collisions) or quantum computing.
                struct QuantumStress {
                    float quantumPressure;    // Quantum mechanical stress
                    float quantumPotential;   // Quantum potential energy
                    float quantumDamping;     // Quantum dissipation rate
                };

                void updatePrincipalStresses() {
                    // Compute principal stresses from tensor
                    principal = computePrincipalStresses(tensor);
                }
                
                void updateStress(const vec3& force, const vec3& area) {
                    // Update stress based on applied forces
                    tensor = computeStressTensor(force, area);
                    updatePrincipalStresses();
                }
            } stress;

            // Field line interaction properties
            struct FieldLineInteraction {
                vec3 fieldLineDirection;     // Local field topology
                float fieldLineDensity;      // Field strength metric
                float crossingAngle;         // Angle between object and field lines
                
                struct ResistanceMetrics {
                    float topologicalDrag;    // Resistance to field line displacement
                    float reconnectionRate;   // Field line reconfiguration speed
                    float pinningStrength;    // How strongly field lines are "held"
                };
            } fieldLineInteraction;

            // Temperature as wave energy density
            struct ThermalField {
                float temperature;        // Local temperature
                vec3 heatFlow;           // Heat propagation direction
                float conductivity;       // Heat transfer rate
                
                void propagateHeat(float deltaTime) {
                    // Heat propagation through wave diffusion
                    float energyDensity = temperature * conductivity;
                    m_solver.propagateEnergyWave(heatFlow, energyDensity);
                }
            } thermal;

            // Fracture through wave discontinuities
            struct FractureField {
                std::vector<vec3> crackTips;    // Active crack positions
                std::vector<vec3> propagation;   // Crack growth directions
                float criticalEnergy;            // Energy for crack growth
                
                void updateCracks(const StressField& stress) {
                    for (size_t i = 0; i < crackTips.size(); i++) {
                        if (computeStrainEnergy(stress) > criticalEnergy) {
                            propagateCrack(i);
                        }
                    }
                }
            } fracture;

            // Methods to compute energy storage and dissipation
            float computeStoredEnergy() const {
                return stateFields.energyStorage.topologicalPotential +
                       stateFields.energyStorage.orientationalEnergy +
                       stateFields.energyStorage.domainWallEnergy;
            }

            float computeResistiveLoss(float deltaTime) const {
                return (stateFields.energyStorage.resistiveEffects.hysteresis +
                        stateFields.energyStorage.resistiveEffects.eddyCurrentLoss) * deltaTime;
            }

            // Compute work done against field
            vec3 computeFieldWork(const vec3& displacement) const {
                vec3 fieldForce = -stateFields.fieldLineInteraction.fieldLineDensity *
                                  stateFields.fieldLineInteraction.fieldLineDirection;
                return fieldForce * displacement;
            }
        } state;
    };

    // Solver configuration for different physics layers
    struct SolverConfig {
        std::string name;  // e.g. "GlobalSolver", "CharacterSolver"
        float resolution;  // Spatial resolution for this solver
        uint32_t maxIterations;
        bool enableQuantization;
        float energyThreshold;
        // ... other solver-specific settings
    };

    // Processing systems all working through the wave-field paradigm
    struct UnifiedProcessing {
        WaveTracker m_waveTracker;           // Tracks wave propagation
        EnergyPropagator m_energyProp;       // Handles energy transfer
        FieldInteractionSolver m_fieldSolver; // Resolves all field interactions
    };

    // Inter-solver communication types
    enum class SolverMessageType {
        ENERGY_TRANSFER,
        WAVE_INTERACTION,
        FIELD_UPDATE,
        BOUNDARY_CONDITION,
        SOLVER_SYNC,
        PRIORITY_UPDATE
    };

    struct SolverMessage {
        SolverMessageType type;
        SolverId sourceSolver;
        SolverId targetSolver;
        
        // Packed message data using std::variant for type safety
        std::variant<
            EnergyTransferData,
            WaveInteractionData,
            FieldUpdateData,
            BoundaryConditionData,
            SolverSyncData,
            PriorityUpdateData
        > data;
        
        float priority;
        uint64_t timestamp;
    };

    // Message queue with priority handling
    class SolverMessageQueue {
    private:
        std::priority_queue<SolverMessage> m_queue;
        std::mutex m_queueMutex;
        
    public:
        void push(SolverMessage&& msg) {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_queue.push(std::move(msg));
        }
        
        bool pop(SolverMessage& msg) {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (m_queue.empty()) return false;
            msg = std::move(m_queue.top());
            m_queue.pop();
            return true;
        }
    };

    // Dynamic thresholds for energy management. Used consciously, only in situations where emergent dynamics, behaviors prove too
    // unreliable, umbearable, etc.
    struct DynamicThresholds {
        // Soft returns using sigmoid-like functions
        float smoothClamp(float value, float min, float max, float smoothness) {
            float range = max - min;
            float normalized = (value - min) / range;
            float sigmoid = 1.0f / (1.0f + exp(-normalized * smoothness));
            return min + (sigmoid * range);
        }
    
        // Energy level management
        struct EnergyBounds {
            float nominal;     // Expected operating range
            float warning;     // Start applying soft limits
            float critical;    // Apply stronger dampening
            float emergency;   // Hard clamp if needed
        } bounds;
    };

    // Bridge between traditional shapes (including colliders) and field representation
    struct ShapeFieldMapping {
        // Field patterns for common shapes
        struct ShapePattern {
            std::vector<vec3> fieldNodes;      // Discretization points (used to place waves)
            std::vector<float> fieldStrengths; // Field intensity at each node (used to modulate waves)
            float boundaryThreshold;           // Surface definition threshold (used to define waves as boundary waves)
            vec3 centerOfField;                // Field center of mass (used to define waves as boundary waves)
        };

        // Shape-specific field generators
        struct FieldGenerators {
            // Generate field patterns for common shapes. We start from the mesh's bounding box to set the extents for cubes and radius for spheres. Then based on grid density
            // calculation algorithms we can generate a recursively refined grid of waves to cover the entire volume.
            static ShapePattern Box(const vec3& size) {
                ShapePattern pattern;
                // Generate field nodes (used to place waves) along box surfaces with higher density at edges
                generateBoxField(size, pattern);
                return pattern;
            }

            static ShapePattern Sphere(float radius) {
                ShapePattern pattern;
                // Generate radially distributed field nodes (waves)
                generateSphereField(radius, pattern);
                return pattern;
            }

            static ShapePattern Capsule(float radius, float height) {
                ShapePattern pattern;

                // Combine cylinder field with hemisphere caps.
                // In the worst case/lower resolution, we can use a single wave to represent each hemisphere, plus N waves for the cylinder, with smaller volume waves to fill in the gaps.
                // This is a trade-off between performance and accuracy, and we can always use a higher resolution to get better results.
                generateCapsuleField(radius, height, pattern);
                return pattern;
            }
        };
    };

    // Wave interference patterns that indicate collisions
    struct CollisionPattern {
        float interferenceIntensity;    // Peak of wave superposition
        float gradientDiscontinuity;    // Sharp changes in field
        vec3 contactNormal;             // Derived from wave front
        float penetrationDepth;         // Field overlap measure
        
        // Energy state at collision point
        struct EnergyState {
            float kineticComponent;     // Motion-related energy
            float potentialBarrier;     // Repulsion energy
            float dissipationRate;      // Energy loss during contact
        } energyState;
    };

    // Multi-level optimization system.
    // TODO: The design sketch went too far, and also created redundant structs, instead of using existing ones. Refactor to be
    // much leaner and concise for now.
    struct WaveOptimizer {
        // Level 1: Local Solver Optimizations
        struct LocalOptimizer {
            // Adaptive sparse resolution control, using activity levels
            struct AdaptiveSparseGrid {
                float baseResolution;
                float minResolution;
                float maxResolution;
                
                // Compute local resolution based on activity level, which in turn is updated based on energy transfer events, events triggered by moving wavefronts, etc.
                // This is the core of the adaptive grid system.
                float computeLocalResolution(const vec3& position) {
                    return std::clamp(baseResolution * activityLevel(position), minResolution, maxResolution);
                }
            } grid;

            // Precision scaling based on energy levels
            struct PrecisionControl {
                enum class Precision {
                    HIGH_32BIT = 32,
                    MEDIUM_16BIT = 16,
                    LOW_8BIT = 8
                };
                
                Precision selectPrecision(float energy) {
                    if (energy > m_highEnergyThreshold) return Precision::HIGH_32BIT;
                    if (energy > m_mediumEnergyThreshold) return Precision::MEDIUM_16BIT;
                    return Precision::LOW_8BIT;
                }
            } precision;

            // SIMD optimization for wave calculations
            struct SIMDProcessor {
                void processWaveBatch(WaveProperty* waves, size_t count) {
                    #ifdef __AVX__
                        processWavesAVX(waves, count);
                    #elif defined(__ARM_NEON)
                        processWavesNEON(waves, count);
                    #else
                        processWavesScalar(waves, count);
                    #endif
                }
            } simd;
        } local;

        // Level 2: Domain-Level Optimization
        struct DomainOptimizer {
            // Spatial partitioning for efficient queries
            struct SpatialCache {
                static constexpr size_t CELL_SIZE = 64;
                std::array<WaveCell, CELL_SIZE> cells;
                
                void updateCell(size_t index, const WaveProperty& wave) {
                    cells[index].addWave(wave);
                    cells[index].optimize();
                }
            } cache;

            // Streaming system for large domains
            struct StreamingSystem {
                static constexpr size_t STREAM_BUFFER_SIZE = 1024;
                CircularBuffer<WaveProperty> streamBuffer;
                
                void streamIn(const vec3& focusPoint) {
                    loadHighPriorityWaves(focusPoint);
                    unloadDistantWaves(focusPoint);
                }
            } streaming;

            // LOD system for distance-based optimization
            struct LODSystem {
                struct LODLevel {
                    float distance;
                    float resolution;
                    uint32_t maxWaves;
                };
                
                std::array<LODLevel, 4> levels;
                
                LODLevel& selectLOD(float distance) {
                    for (auto& level : levels) {
                        if (distance <= level.distance) return level;
                    }
                    return levels.back();
                }
            } lod;
        } domain;

        // Level 3: Global System Optimization
        struct GlobalOptimizer {
            // Dynamic field configuration, including EM and fluid fields
            struct FieldInfo {
                // Core (always enabled)
                bool mechanical = true;  // Disabling will turn off physics collisions, solidBodyNode dynamics
                
                // Optional fields
                struct EMFieldInfo {
                    bool enabled = false;
                    float minEnergyThreshold = 0.01f;
                    bool lightInteraction = false;
                    bool radiationEffects = false;
                } electromagnetic;
                
                struct FluidFieldInfo {
                    bool enabled = false;
                    bool gasSimulation = false;
                    bool liquidSimulation = false;
                    float densityThreshold = 0.1f;
                } fluid;

                // For games, entertainment. Maybe we can skip this one and use EM fields instead, for magic fx
                struct MagicFieldInfo {
                    bool enabled = false;
                    float minEnergyThreshold = 0.001f;
                    bool spellInteraction = false;
                    bool auraEffects = false;
                } magic;
                
                // Auto-detection settings
                struct AutoDetectionInfo {
                    bool autoDetectTechLevel = true;
                    bool autoEnableForMagic = true;
                    float activationRadius = 100.0f;
                } autoDetection;
            } fieldInfo;

            // Memory management
            struct MemoryManager {
                static constexpr size_t POOL_SIZE = 16384;
                ObjectPool<WaveProperty> wavePool;
                
                template<typename... Args>
                WaveProperty* allocateWave(Args&&... args) {
                    return wavePool.allocate(std::forward<Args>(args)...);
                }
            } memory;

            // Thread management for parallel processing
            struct ThreadManager {
                ThreadPool workers;
                
                void scheduleWaveProcessing(const std::vector<WaveProperty*>& waves) {
                    size_t batchSize = waves.size() / workers.threadCount();
                    for (size_t i = 0; i < workers.threadCount(); i++) {
                        workers.enqueue([=] {
                            processWaveBatch(waves, i * batchSize, (i + 1) * batchSize);
                        });
                    }
                }
            } threads;

            // Load balancing between CPU/GPU
            struct LoadBalancer {
                float cpuLoad;
                float gpuLoad;
                
                void balanceLoad() {
                    if (cpuLoad > gpuLoad * 1.2f) {
                        moveWorkToGPU();
                    } else if (gpuLoad > cpuLoad * 1.2f) {
                        moveWorkToCPU();
                    }
                }
            } balance;

            void updateFieldConfigurations(const vec3& position) {
                // Check for auto-detection, using effective rules
                // TODO
            }
        } global;

        // Level 4: Emergency Fallback System
        struct EmergencySystem {
            // Performance monitoring
            struct Monitor {
                float fps;
                float frameTime;
                float memoryUsage;
                
                bool needsFallback() const {
                    return fps < 30.0f || frameTime > 33.3f || memoryUsage > 0.9f;
                }
            } monitor;

            // Fallback strategies
            struct FallbackStrategies {
                void applyFallback(const Monitor& monitor) {
                    if (monitor.needsFallback()) {
                        reducePrecision();
                        increaseThresholds();
                        disableNonEssentialEffects();
                    }
                }
            } fallback;
        } emergency;
    } m_optimizer;

    // === Allocation, Initialization, Loading ===
    explicit WavePhysics(const SolverConfig& config = SolverConfig()) 
        : Node(config.name), m_config(config) {}

    // Energy representation
    EnergySpectrum m_energy;
    FieldCoupling m_coupling;

    // Energy field representation - versatile approach
    EnergyField m_energyField;

    // Energy fields - specialized
    EMField m_emField;
    GravitationalField m_gravitationalField;
    QuantumField m_quantumField;
    FluidField m_fluidField;
    GasField m_gasField;
    SolidField m_solidBodyField;

    // Single unified solver handling all physics through wave-field interactions
    WavePhysicsSolver m_solver;
    int substeps = 1;
    int simulationRate = 60;
    float timeStep = 1.0f / simulationRate;
    float gravityApprox = 9.81f;
    bool enableContinuousDetection = true;

    // Solid body behavior emerges from field properties
    void setSolidProperties(const Solid* node, float elasticity) {
        PhysicsField::SolidField field;
        field.fieldStiffness = elasticity;
        m_solver.updateLocalField(node->getWorldPosition(), field);
    }

    // === Physics System Management ===
    // Inter-solver communication methods
    void connectSolvers(SolverId source, SolverId target) {
        auto& sourceRef = m_solvers[source];
        auto& targetRef = m_solvers[target];
        
        // Register bidirectional connection
        m_solverConnections[source].push_back(target);
        m_solverConnections[target].push_back(source);
        
        // Setup shared boundary conditions
        setupSharedBoundary(sourceRef, targetRef);
    }

    void disconnectSolvers(SolverId source, SolverId target) {
        // Remove bidirectional connection
        auto& sourceConns = m_solverConnections[source];
        auto& targetConns = m_solverConnections[target];
        
        sourceConns.erase(
            std::remove(sourceConns.begin(), sourceConns.end(), target),
            sourceConns.end()
        );
        
        targetConns.erase(
            std::remove(targetConns.begin(), targetConns.end(), source),
            targetConns.end()
        );
    }

    // Send message between solvers
    void sendSolverMessage(SolverMessage&& msg) {
        if (!validateMessage(msg)) return;
        
        // Handle immediate messages
        if (msg.priority > 0.9f) {
            processSolverMessage(msg);
        } else {
            m_messageQueue.push(std::move(msg));
        }
    }

    // === Processing ===
    // Energy transfer and transformation
    void transferEnergy(const vec3& position, float amount, 
                       EnergyManifestationType::Type sourceType,
                       EnergyManifestationType::Type targetType);
                       
    // Automatic reaction spawning
    void spawnReactions(const vec3& position, float energy);

    // Propagate wave from impact
    void propagateWave(const vec3& origin, const vec3& direction, float initialEnergy) {
        WaveProperty wave;
        wave.amplitude = initialEnergy;
        wave.propagationDirection = direction;
        wave.frequency = calculateInitialFrequency(initialEnergy);
        wave.effects = wave.calculateEffects();
        
        // Create wavefront tracker
        auto wavefront = m_particleSystem.createParticle(origin, initialEnergy);
        wavefront.pathType = ShortLivedParticle::Path::FIELD_GUIDED;
        
        // Consider frequency-dependent effects
        if (wave.effects.canTriggerDecay) {
            initializeDecayChain(wave);
        }
        
        // Register wave contribution with penetration effects
        float radius = calculateInfluenceRadius(initialEnergy);
        addWaveContribution(origin, radius, wave);
    }

    // Compute superposition at point
    float computeFieldStrength(const vec3& point) {
        float totalEnergy = 0.0f;
        
        for (const auto& wave : m_activeWaves) {
            // Consider phase and attenuation
            float distance = glm::length(point - wave.origin);
            float attenuation = calculateAttenuation(distance, wave.medium);
            float phaseShift = distance * wave.frequency;
            
            totalEnergy += wave.amplitude * attenuation * 
                          std::cos(phaseShift + wave.phase);
        }
        
        return totalEnergy;
    }

    // Handle wave interactions
    void processWaveInteractions() {
        for (const auto& wave : m_activeWaves) {
            // Check for constructive/destructive interference
            processInterference(wave);
            
            // Handle medium transitions
            processMediumBoundaries(wave);
            
            // Update wave properties based on field conditions
            updateWaveProperties(wave);
        }
    }

    void updateFieldInteraction(const MaterialField& material, float deltaTime) {
        // Calculate stored energy from field resistance
        float storedEnergy = material.computeStoredEnergy();
        
        // Generate standing waves to represent stored potential
        if (storedEnergy > m_config.minEnergyThreshold) {
            createStandingWaves(
                material.stateFields.fieldLineInteraction.fieldLineDirection,
                storedEnergy,
                material.stateFields.fieldLineInteraction.fieldLineDensity
            );
        }
        
        // Handle energy dissipation through wave propagation
        float dissipatedEnergy = material.computeResistiveLoss(deltaTime);
        if (dissipatedEnergy > 0) {
            propagateEnergyWaves(
                material.stateFields.fieldLineInteraction.fieldLineDirection,
                dissipatedEnergy,
                WaveType::DISSIPATIVE
            );
        }
    }

    // Relativistic mass adjustment (can be toggled for performance)
    float calculateRelativisticMass(float restMass, const vec3& velocity) {
        if (!config.enableRelativisticEffects || glm::length(velocity) < config.relativisticThreshold) {
            return restMass; // Skip calculation for low velocities
        }
        
        float speedRatio = glm::length(velocity) / SPEED_OF_LIGHT;
        float gamma = 1.0f / std::sqrt(1.0f - speedRatio * speedRatio);
        return restMass * gamma;
    }

    // Time dilation between reference frames
    float calculateTimeDilation(const vec3& relativeVelocity) {
        if (!config.enableRelativisticEffects) {
            return 1.0f; // No dilation when disabled
        }
        
        float speedRatio = glm::length(relativeVelocity) / SPEED_OF_LIGHT;
        return std::sqrt(1.0f - speedRatio * speedRatio); // Time runs slower by this factor
    }

    // Enforce light-speed limit on propagation
    vec3 enforceSpeedLimit(const vec3& velocity) {
        float speed = glm::length(velocity);
        if (speed > SPEED_OF_LIGHT * config.lightSpeedFactor) {
            return velocity * (SPEED_OF_LIGHT * config.lightSpeedFactor / speed);
        }
        return velocity;
    }

    void updatePhysics(float deltaTime) {
        // All physics phenomena processed through wave-field interactions
        m_solver.propagateWaves(deltaTime);
        m_solver.resolveFieldInteractions(deltaTime);
        m_solver.updateEnergyStates(deltaTime);
    }

    // === Debugging & Visualization ===
    void debugVisualizeWave() {
        // Visualize wave propagation and interactions
        // for debugging and analysis
    }

    void debugVisualizeField() {
        // Visualize field strength and wave properties
        // for debugging and analysis
    }

    void debugVisualizeWavefronts() {
        // Visualize wavefronts and their propagation paths
        // for debugging and analysis
    }

    void debugVisualizeReactions() {
        // Visualize spawned reactions and their effects
        // for debugging and analysis
    }

    // TODO: Move these to the .cpp file
    // Wave propagation visualization
    void debugVisualizeWavePropagation(const Wave& wave, DebugRenderer& renderer) {
        // Draw wave direction
        renderer.drawArrow(wave.position, 
                        wave.position + wave.propagation * wave.amplitude, 
                        Color(0.2f, 0.6f, 1.0f, 0.8f));
        
        // Draw amplitude as sphere
        renderer.drawSphere(wave.position, wave.amplitude * 0.1f, 
                        Color(1.0f, 0.5f, 0.2f, 0.5f));
        
        // Draw frequency as rings
        float wavelength = wave.velocity / wave.frequency;
        for (int i = 1; i <= 3; i++) {
            renderer.drawCircle(wave.position, wavelength * i, 
                            wave.propagation, 
                            Color(0.2f, 1.0f, 0.5f, 0.7f / i));
        }
    }

    // Energy conservation visualization
    void debugVisualizeEnergyTransfer(const EnergyTransferEvent& event, DebugRenderer& renderer) {
        // Draw source energy
        renderer.drawSphere(event.position, 0.2f, Color(1.0f, 0.0f, 0.0f, 0.7f));
        
        // Draw text with energy value
        renderer.drawText(event.position + vec3(0.0f, 0.3f, 0.0f), 
                        std::to_string(event.sourceEnergy) + " J", 
                        Color(1.0f, 1.0f, 1.0f, 1.0f));
        
        // Draw output energies
        float totalOutput = 0.0f;
        for (const auto& output : event.outputs) {
            renderer.drawArrow(event.position, output.position, 
                            Color(0.0f, 1.0f, 0.0f, 0.7f));
            
            renderer.drawText(output.position, 
                            std::to_string(output.energy) + " J", 
                            Color(1.0f, 1.0f, 1.0f, 1.0f));
            
            totalOutput += output.energy;
        }
        
        // Draw conservation status
        float ratio = totalOutput / event.sourceEnergy;
        Color conservationColor = (ratio > 0.95f && ratio < 1.05f) ? 
                                Color(0.0f, 1.0f, 0.0f, 1.0f) : 
                                Color(1.0f, 0.0f, 0.0f, 1.0f);
        
        renderer.drawText(event.position + vec3(0.0f, 0.5f, 0.0f), 
                        "Conservation: " + std::to_string(ratio * 100.0f) + "%", 
                        conservationColor);
    }

    // Convert traditional rigid body to wave field representation
    void convertRigidBodyToWaveField(const RigidBody& body, SolidField& field) {
        // Create a sparse field representation centered on the rigid body
        field.clear();
        
        // Set up basic properties
        field.centerOfMass = body.position;
        field.totalMass = body.mass;
        
        // Create energy distribution based on shape
        switch (body.shapeType) {
            case ShapeType::Box: {
                // For box, create 8 corner points and center
                vec3 halfExtents = body.dimensions * 0.5f;
                for (int x = -1; x <= 1; x += 2) {
                    for (int y = -1; y <= 1; y += 2) {
                        for (int z = -1; z <= 1; z += 2) {
                            vec3 localPoint = vec3(x * halfExtents.x, 
                                                y * halfExtents.y, 
                                                z * halfExtents.z);
                            vec3 worldPoint = body.position + 
                                            body.rotation * localPoint;
                            
                            // Add energy point
                            field.addEnergyPoint(worldPoint, body.mass / 8.0f);
                        }
                    }
                }
                // Add center point
                field.addEnergyPoint(body.position, body.mass / 8.0f);
                break;
            }
            // Add other shape types...
        }
        
        // Set momentum based on velocity
        field.momentumPotential.setUniform(body.velocity * body.mass);
        
        // Set angular momentum
        field.angularMomentumPotential.setUniform(body.angularVelocity * body.inertia);
    }

    // Convert wave field back to traditional rigid body
    void convertWaveFieldToRigidBody(const SolidField& field, RigidBody& body) {
        // Extract center of mass
        body.position = field.computeCenterOfMass();
        
        // Extract total mass
        body.mass = field.computeTotalMass();
        
        // Extract linear momentum and compute velocity
        vec3 totalMomentum = field.momentumPotential.computeTotal();
        body.velocity = totalMomentum / body.mass;
        
        // Extract angular momentum and compute angular velocity
        vec3 totalAngularMomentum = field.angularMomentumPotential.computeTotal();
        body.angularVelocity = totalAngularMomentum / body.inertia;
        
        // Estimate shape from energy distribution
        estimateShapeFromEnergyField(field, body);
    }

private:
    // === Allocation, Initialization, Loading ===
    std::vector<WaveProperty> m_activeWaves;
    ShortLivedParticleSystem m_particleSystem;  // Used for wavefront tracking

    void initializeDecayChain(const WaveProperty& wave) {
        if (Random::Float() < wave.effects.chainReactionProbability) {
            // Spawn secondary radiation waves
            spawnSecondaryWaves(wave);
        }
    }

    // Create and configure a new solver instance
    SolverId createSolver(const SolverConfig& config) {
        auto solver = std::make_unique<WavePhysicsSolver>(config);
        SolverId id = m_nextSolverId++;
        m_solvers[id] = std::move(solver);
        return id;
    }

    void spawnSecondaryWaves(const WaveProperty& primary) {
        // Simulate particle decay or nuclear reactions
        // producing secondary radiation
    }

    void addWaveContribution(const vec3& origin, float radius, const WaveProperty& wave) {
        // Add wave contribution to the field
        // Consider medium properties and attenuation
    }

    // === Processing ===
    float calculateInitialFrequency(float initialEnergy) {
        // Map initial energy to frequency
        // Consider different manifestation types
        return initialEnergy * m_config.frequencyScale;
    }

    float calculateAttenuation(float distance, const WaveProperty::MediumInteraction& medium) {
        return std::exp(-distance * medium.attenuation);
    }

    float calculateInfluenceRadius(float initialEnergy) {
        // Determine influence radius based on energy
        return initialEnergy * m_config.radiusScale;
    }

    void processInterference(const WaveProperty& wave) {
        // Handle wave superposition and interference patterns
        // This affects energy distribution in the field
    }

    void processMediumBoundaries(const WaveProperty& wave) {
        // Handle reflection, refraction, and transmission
        // at medium boundaries
    }

    void updateWaveProperties(WaveProperty& wave) {
        // Adjust wave properties based on field conditions
        // and energy levels
    }

    // Process queued messages
    void processMessageQueue() {
        SolverMessage msg;
        while (m_messageQueue.pop(msg)) {
            processSolverMessage(msg);
        }
    }

    void processSolverMessage(const SolverMessage& msg) {
        auto& sourceSolver = m_solvers[msg.sourceSolver];
        auto& targetSolver = m_solvers[msg.targetSolver];
        
        switch (msg.type) {
            case SolverMessageType::ENERGY_TRANSFER: {
                const auto& data = std::get<EnergyTransferData>(msg.data);
                transferEnergyBetweenSolvers(sourceSolver, targetSolver, data);
                break;
            }
            case SolverMessageType::WAVE_INTERACTION: {
                const auto& data = std::get<WaveInteractionData>(msg.data);
                handleWaveInteraction(sourceSolver, targetSolver, data);
                break;
            }
            case SolverMessageType::FIELD_UPDATE: {
                const auto& data = std::get<FieldUpdateData>(msg.data);
                updateSharedField(sourceSolver, targetSolver, data);
                break;
            }
            // ... handle other message types
        }
    }

    void setupSharedBoundary(WavePhysicsSolver& source, WavePhysicsSolver& target) {
        // Create shared boundary region
        auto boundary = std::make_shared<BoundaryRegion>();
        boundary->resolution = std::min(source.getResolution(), target.getResolution());
        
        // Setup interpolation for different resolutions
        if (source.getResolution() != target.getResolution()) {
            boundary->setupInterpolation(
                source.getResolution(),
                target.getResolution()
            );
        }
        
        // Register boundary with both solvers
        source.addBoundaryRegion(boundary);
        target.addBoundaryRegion(boundary);
    }

    bool validateMessage(const SolverMessage& msg) {
        // Validate solver IDs exist
        if (!m_solvers.contains(msg.sourceSolver) || 
            !m_solvers.contains(msg.targetSolver)) {
            return false;
        }
        
        // Check if solvers are connected
        auto it = m_solverConnections.find(msg.sourceSolver);
        if (it == m_solverConnections.end()) return false;
        
        return std::find(it->second.begin(), 
                        it->second.end(), 
                        msg.targetSolver) != it->second.end();
    }

    // Example of mass behavior emerging from field properties
    void updateMassBehavior(const vec3& position, float deltaTime) {
        auto localField = m_solver.getLocalField(position);
        
        // Mass-like behavior emerges from energy density patterns
        EmergentBehaviors::MassPattern mass;
        mass.localEnergyDensity = localField.computeEnergyDensity();
        mass.inertialResistance = localField.getFieldStability();
        mass.energyGradient = localField.getEnergyGradient();

        // Update field to maintain mass-like properties
        if (mass.localEnergyDensity > 0.0f) {
            // Resist changes based on inertial properties
            vec3 fieldForce = computeExternalForces(position);
            vec3 inertialResponse = -fieldForce * mass.inertialResistance;
            
            // Apply inertial response through wave propagation
            m_solver.propagateWave(
                position,
                inertialResponse,
                WaveType::INERTIAL_RESPONSE
            );
        }
    }

    // Solid body behavior emerging from field coupling
    void updateSolidBehavior(const Solid* node, float deltaTime) {
        auto worldPos = node->getWorldPosition();
        auto& cohesion = node->getCohesionPattern();

        // Strong field coupling creates rigid-like behavior
        if (cohesion.bondStrength > 0.9f) {  // Nearly rigid
            // Maintain strict relative positions through strong field forces
            enforceRigidConstraints(node);
        } else {  // More elastic
            // Allow controlled deformation through field oscillations
            propagateElasticWaves(node, cohesion.internalResonance);
        }
    }

    // Motion behavior emerging from wave propagation
    void updateMotionBehavior(const vec3& position, const vec3& velocity, float deltaTime) {
        EmergentBehaviors::MotionPattern motion;
        
        // Convert traditional velocity to wave properties
        motion.momentumWave = createMomentumWave(velocity);
        motion.phaseVelocity = calculatePhaseVelocity(velocity.length());
        
        // Propagate motion through field
        m_solver.propagateWave(
            position,
            motion.momentumWave,
            WaveType::MOMENTUM_CARRIER
        );

        // Update position based on group velocity
        vec3 newPosition = position + 
            (motion.groupVelocity * motion.momentumWave.normalized() * deltaTime);
        
        // Ensure conservation of momentum through field interactions
        conserveMomentum(position, newPosition, motion.momentumWave);
    }

private:
    void enforceRigidConstraints(const Solid* node) {
        // Create standing waves that maintain relative positions
        auto vertices = node->getVertices();
        for (const auto& vertex : vertices) {
            // High-frequency, low-amplitude waves create rigid-like bonds
            m_solver.createStandingWave(
                vertex.position,
                FrequencyBand::kHighFrequency,
                AmplitudeBand::kStructural
            );
        }
    }

    void propagateElasticWaves(const Solid* node, float resonance) {
        // Create waves that allow controlled deformation
        auto vertices = node->getVertices();
        for (const auto& vertex : vertices) {
            // Lower frequency waves allow more flexibility
            m_solver.createOscillatingWave(
                vertex.position,
                resonance,
                AmplitudeBand::kElastic
            );
        }
    }

    vec3 createMomentumWave(const vec3& velocity) {
        // Convert linear velocity to wave momentum
        return WavePattern::createDirectional(
            velocity,
            FrequencyBand::kMomentumCarrier,
            velocity.length() * m_energyScale
        );
    }

    /**
     * @brief Detects collisions through wave-field interference patterns.
     */
    void detectCollisions() {
        // Analyze field for interference patterns
        auto& grid = m_solver.getAdaptiveGrid();
        
        for (const auto& cell : grid.getActiveCells()) {
            // Look for characteristic collision patterns
            CollisionPattern pattern = analyzeInterference(cell);
            auto validation = validateCollision(cell.event);
            
            if (pattern.interferenceIntensity > m_thresholds.collisionThreshold) {
                // Collision detected - handle response
                handleCollision(cell.event, pattern, validation);
            }
        }
    }

    /**
     * @brief Handles collision response based on wave-field patterns.
     * Use a traditional physics fallback for simple cases, wave physics for simple to complex interactions.
     * The idea is to use Adaptive Wave Density, LOD for wave physics. We resort to traditional physics only to make sure we don't miss any collisions.
     * @params event: The collision event (which has comprehensive information about the collision), pattern: The detected collision pattern, validation: The validation result.
     */
    void handleCollision(const CollisionEvent& event,
            const CollisionPattern& pattern,
            const ValidationResult& validation) {
        // Use traditional physics fallback, if enabled as a backup strategy for simple cases.
        if (m_useTraditionalPhysicsFallback && validation.confidence < m_config.minWaveFieldThreshold) {
            applyGeometricResponse(event, computeGeometricSeparation(event));
            return;
        }

        // Use wave physics interference for complex interactions
        vec3 responseWave = computeWaveFieldResponse(event);

        // Spawn both compression and shear waves
        spawnCollisionWaves(event.position, responseWave);
        
        // Transfer energy between colliding bodies
        transferCollisionEnergy(event.position, pattern);
        
        // Generate secondary effects (deformation, particles)
        generateCollisionEffects(event.position, pattern);
    }

private:
    CollisionPattern analyzeInterference(const GridCell& cell) {
        CollisionPattern pattern;
        
        // Analyze wave superposition
        auto waves = m_solver.getWavesInCell(cell);
        pattern.interferenceIntensity = computeInterference(waves);
        
        // Detect field discontinuities
        pattern.gradientDiscontinuity = 
            m_solver.computeFieldGradient(cell.position).length();
            
        // Determine contact properties
        if (pattern.interferenceIntensity > 0) {
            pattern.contactNormal = 
                -m_solver.getFieldGradientDirection(cell.position);
            pattern.penetrationDepth = 
                computePenetrationFromField(cell);
        }
        
        return pattern;
    }

    void spawnCollisionWaves(const vec3& position, const WaveProperty& response) {
        // Primary compression wave
        m_solver.propagateWave(
            position,
            response.direction,
            WaveType::COMPRESSION,
            response.amplitude
        );
        
        // Secondary shear waves
        vec3 tangent = computeTangentVector(response.direction);
        m_solver.propagateWave(
            position,
            tangent,
            WaveType::SHEAR,
            response.amplitude * 0.5f
        );
    }

    void transferCollisionEnergy(const vec3& position, 
                               const CollisionPattern& pattern) {
        // Calculate energy exchange based on field properties
        float exchangeRate = computeEnergyExchangeRate(pattern);
        
        // Create energy transfer waves
        for (const auto& direction : getEnergyTransferDirections(pattern)) {
            m_solver.propagateEnergyWave(
                position,
                direction,
                pattern.energyState.kineticComponent * exchangeRate
            );
        }
    }

    void generateCollisionEffects(const vec3& position, 
                                const CollisionPattern& pattern) {
        // Spawn deformation waves if energy is high enough
        if (pattern.energyState.kineticComponent > 
            m_thresholds.deformationThreshold) {
            spawnDeformationWaves(position, pattern);
        }
        
        // Create particle effects through field excitation
        if (pattern.interferenceIntensity > 
            m_thresholds.particleEmissionThreshold) {
            emitParticles(position, pattern);
        }
    }

    void spawnDeformationWaves(const vec3& position, 
                             const CollisionPattern& pattern) {
        // Create internal stress waves
        WaveProperty stressWave;
        stressWave.amplitude = pattern.energyState.kineticComponent * 0.3f;
        stressWave.frequency = calculateStressFrequency(pattern);
        
        // Propagate through material
        m_solver.propagateInternalWave(
            position,
            stressWave,
            WaveType::DEFORMATION
        );
    }

    void emitParticles(const vec3& position, const CollisionPattern& pattern) {
        // Convert excess energy to particle emission
        float particleEnergy = 
            pattern.energyState.kineticComponent * 
            m_config.particleEmissionRatio;
            
        // Create particle emission waves
        m_solver.createParticleEmitter(
            position,
            pattern.contactNormal,
            particleEnergy
        );
    }

    // Integration with traditional collision shapes
    void initializeCollisionShape(const ColliderNode* collider) {
        ShapeFieldMapping::ShapePattern fieldPattern;

        // Convert traditional shape to field pattern
        switch (collider->getShapeType()) {
            case ShapeType::Box: {
                auto& box = static_cast<const BoxCollider*>(collider);
                fieldPattern = ShapeFieldMapping::FieldGenerators::Box(box->getSize());
                break;
            }
            case ShapeType::Sphere: {
                auto& sphere = static_cast<const SphereCollider*>(collider);
                fieldPattern = ShapeFieldMapping::FieldGenerators::Sphere(sphere->getRadius());
                break;
            }
            case ShapeType::Capsule: {
                auto& capsule = static_cast<const CapsuleCollider*>(collider);
                fieldPattern = ShapeFieldMapping::FieldGenerators::Capsule(
                    capsule->getRadius(), 
                    capsule->getHeight()
                );
                break;
            }
        }

        // Create field representation
        createShapeField(collider, fieldPattern);
    }

private:
    void createShapeField(const ColliderNode* collider, 
                         const ShapeFieldMapping::ShapePattern& pattern) {
        // Create standing waves that define the shape's boundary
        for (size_t i = 0; i < pattern.fieldNodes.size(); i++) {
            vec3 worldPos = collider->transformToWorld(pattern.fieldNodes[i]);
            float strength = pattern.fieldStrengths[i];

            // Create boundary-defining waves
            WaveProperty boundaryWave;
            boundaryWave.amplitude = strength;
            boundaryWave.frequency = m_config.boundaryWaveFrequency;

            m_solver.createStandingWave(
                worldPos,
                boundaryWave,
                WaveType::BOUNDARY
            );
        }
    }

/**
 * @brief BoundaryWaveSystem handles boundary wave generation and management.
 */
class BoundaryWaveSystem {
    struct BoundaryDefinition {
        // Primary high-frequency carrier wave
        struct CarrierWave {
            float frequency = FrequencyBand::kBoundaryCarrier; // ~10kHz
            float amplitude = AmplitudeBand::kStructural;      // High amplitude for clear boundaries
            vec3 direction;                                    // Normal to surface
        };

        // Secondary shape-maintaining waves
        struct ShapeWave {
            float frequency = FrequencyBand::kShapeMaintainer; // ~5kHz
            float amplitude = AmplitudeBand::kInternal;        // Lower amplitude, internal structure
            float phase;                                       // Phase offset for standing wave
        };

        // Precision control
        struct PrecisionZone {
            float primaryFrequency;   // Highest frequency for critical areas
            float secondaryFrequency; // Background frequency
            float transitionWidth;    // Smooth frequency transition
        };
    };

    void setupBoundaryWaves(const ColliderNode* collider) {
        auto shape = collider->getShape();
        
        // Create primary boundary waves
        for (const auto& vertex : shape.getVertices()) {
            createBoundaryWave(
                vertex.position,
                vertex.normal,
                BoundaryWaveType::PRIMARY
            );
        }

        // Create shape-maintaining internal waves
        createShapeMaintainingField(shape);
        
        // Setup precision zones
        setupPrecisionZones(shape);
    }
};

/**
 * @brief HybridCollisionSystem handles hybrid collision detection.
 */
class HybridCollisionSystem {
    struct ValidationLayer {
        // Traditional collision data
        struct GeometricBounds {
            BoundingBox aabb;
            BoundingSphere sphere;
            ConvexHull hull;
        };

        // Wave-field validation
        struct WaveFieldMetrics {
            float fieldIntensity;    // Field strength at boundary
            float gradientMagnitude; // Rate of field change
            float coherence;         // Wave pattern stability
        };

        // Hybrid validation results
        struct ValidationResult {
            bool geometricValid;     // Traditional collision check passed
            bool fieldValid;         // Wave-field boundary is stable
            float confidence;        // Combined validation score
        };
    };

    ValidationResult validateCollision(const CollisionEvent& event) {
        ValidationResult result;

        // 1. Quick AABB/Sphere test
        result.geometricValid = checkGeometricBounds(event);
        if (!result.geometricValid) return result;

        // 2. Wave-field boundary validation
        WaveFieldMetrics metrics = analyzeFieldBoundary(event);
        result.fieldValid = validateFieldMetrics(metrics);

        // 3. Compute confidence score
        result.confidence = computeHybridConfidence(
            result.geometricValid,
            result.fieldValid,
            metrics
        );

        return result;
    }

/**
 * @brief HybridSamplingSystem handles adaptive mesh sampling and wave conversion.
 * Sampling Strategies:
 *      - Curvature-Based Sampling, using curvature to guide sampling density.
 *      - Feature-Preserving Decimation, focusing on preserving key features.
 */
class HybridSamplingSystem {
    struct SamplingConfig {
        float minSampleDensity;    // Minimum samples per unit
        float curvatureThreshold;  // Curvature-based sampling
        float featureThreshold;    // Feature preservation threshold
        uint32_t maxSamples;      // Upper bound for performance
    };

    // Wave interference detection
    class WaveInterferenceDetector {
        struct WaveField {
            float amplitude;
            float frequency;
            float phase;
            vec3 direction;
            
            float evaluateAt(const vec3& point) const {
                float distance = glm::length(point - origin);
                float attenuation = 1.0f / (1.0f + distance * distance);
                float wavePhase = frequency * distance + phase;
                return amplitude * attenuation * std::cos(wavePhase);
            }
        };

        struct InterferencePoint {
            vec3 position;
            float fieldA;
            float fieldB;
            float combinedIntensity;
            vec3 gradientDirection;
        };

        std::vector<vec3> getSamplingPoints(const WaveField& fieldA, 
                                          const WaveField& fieldB) {
            std::vector<vec3> points;
            
            // 1. Find overlap region between fields
            BoundingBox overlapRegion = computeFieldOverlap(fieldA, fieldB);
            
            // 2. Create sampling grid in overlap region
            vec3 gridSize = overlapRegion.size() / m_config.samplingResolution;
            for (float x = 0; x < m_config.samplingResolution; x++) {
                for (float y = 0; y < m_config.samplingResolution; y++) {
                    for (float z = 0; z < m_config.samplingResolution; z++) {
                        points.push_back(
                            overlapRegion.min + gridSize * vec3(x, y, z)
                        );
                    }
                }
            }
            
            return points;
        }

        float computeWaveSuperposition(float fieldA, float fieldB) {
            // Basic linear superposition
            float linear = fieldA + fieldB;
            
            // Consider constructive/destructive interference
            float product = fieldA * fieldB;
            
            // Weighted combination based on field properties
            return linear * 0.7f + product * 0.3f;
        }

        vec3 computeGradientDirection(const WaveField& fieldA,
                                    const WaveField& fieldB,
                                    const vec3& point) {
            const float h = 0.01f; // Small offset for gradient computation
            
            // Sample points around center for gradient
            std::array<vec3, 6> offsets = {
                vec3(h, 0, 0), vec3(-h, 0, 0),
                vec3(0, h, 0), vec3(0, -h, 0),
                vec3(0, 0, h), vec3(0, 0, -h)
            };
            
            vec3 gradient(0);
            for (int i = 0; i < 6; i += 2) {
                float pos = computeWaveSuperposition(
                    fieldA.evaluateAt(point + offsets[i]),
                    fieldB.evaluateAt(point + offsets[i])
                );
                float neg = computeWaveSuperposition(
                    fieldA.evaluateAt(point + offsets[i + 1]),
                    fieldB.evaluateAt(point + offsets[i + 1])
                );
                gradient[i/2] = (pos - neg) / (2.0f * h);
            }
            
            return glm::normalize(gradient);
        }

        float estimatePenetrationDepth(const WaveField& fieldA,
                                     const WaveField& fieldB,
                                     const vec3& point) {
            // Follow gradient direction until field strength drops below threshold
            vec3 gradDir = computeGradientDirection(fieldA, fieldB, point);
            float depth = 0.0f;
            float step = 0.01f;
            
            vec3 currentPoint = point;
            float intensity = computeWaveSuperposition(
                fieldA.evaluateAt(currentPoint),
                fieldB.evaluateAt(currentPoint)
            );
            
            while (intensity > m_config.penetrationThreshold && 
                   depth < m_config.maxPenetrationDepth) {
                currentPoint += gradDir * step;
                intensity = computeWaveSuperposition(
                    fieldA.evaluateAt(currentPoint),
                    fieldB.evaluateAt(currentPoint)
                );
                depth += step;
            }
            
            return depth;
        }
    };
};

    // Runtime conversion of vertices to wave sources
    void convertVerticesToWaveSources(const std::vector<CurvatureSample>& samples) {
        for (const auto& sample : samples) {
            // Create primary boundary wave
            WaveSource boundaryWave;
            boundaryWave.frequency = calculateAdaptiveFrequency(sample.curvature);
            boundaryWave.amplitude = calculateAmplitude(sample.importance);
            boundaryWave.position = sample.position;
            boundaryWave.direction = sample.normal;

            m_waveSources.push_back(boundaryWave);
        }
    }

    // Collision detection through wave interference
    struct InterferencePattern {
        float intensity;          // Wave superposition strength
        vec3 interferencePoint;   // Location of interference
        vec3 gradientDirection;   // Direction of strongest gradient
        float penetrationDepth;   // Based on field overlap
    };

    std::vector<InterferencePattern> detectCollisions() {
        std::vector<InterferencePattern> collisions;
        
        // 1. Broad phase: Check wave source proximity
        auto potentialPairs = m_broadphase.findPotentialPairs(m_waveSources);
        
        // 2. Narrow phase: Analyze wave interference
        for (const auto& pair : potentialPairs) {
            // Calculate wave interference at sampling points
            auto interference = analyzeWaveInterference(
                pair.first.getWaveField(),
                pair.second.getWaveField()
            );
            
            // Check for significant interference patterns
            if (interference.intensity > m_config.collisionThreshold) {
                collisions.push_back(interference);
            }
        }
        
        return collisions;
    }

    // Analyze wave interference between two fields
    InterferencePattern analyzeWaveInterference(
        const WaveField& fieldA, 
        const WaveField& fieldB) {
        
        InterferencePattern pattern;
        
        // Find points of maximum wave superposition
        for (const auto& point : getSamplingPoints(fieldA, fieldB)) {
            float superposition = computeWaveSuperposition(
                fieldA.evaluateAt(point),
                fieldB.evaluateAt(point)
            );
            
            if (superposition > pattern.intensity) {
                pattern.intensity = superposition;
                pattern.interferencePoint = point;
                pattern.gradientDirection = 
                    computeGradientDirection(fieldA, fieldB, point);
                pattern.penetrationDepth = 
                    estimatePenetrationDepth(fieldA, fieldB, point);
            }
        }
        
        return pattern;
    }
};

    ValidationResult validateCollision(const CollisionEvent& event) {
        ValidationResult result;

        // 1. Quick AABB/Sphere test
        result.geometricValid = checkGeometricBounds(event);
        if (!result.geometricValid) return result;

        // 2. Wave-field boundary validation
        WaveFieldMetrics metrics = analyzeFieldBoundary(event);
        result.fieldValid = validateFieldMetrics(metrics);

        // 3. Compute confidence score
        result.confidence = computeHybridConfidence(
            result.geometricValid,
            result.fieldValid,
            metrics
        );

        return result;
    }

    void handleCollision(const CollisionEvent& event) {
        auto validation = validateCollision(event);
        
        if (validation.confidence > m_config.confidenceThreshold) {
            // High confidence: Use pure wave-field physics
            handleWaveFieldCollision(event);
        } else {
            // Low confidence: Use hybrid approach
            handleHybridCollision(event, validation);
        }
    }

private:
    void handleHybridCollision(const CollisionEvent& event, 
                              const ValidationResult& validation) {
        // Start with traditional collision response
        vec3 separationVector = computeGeometricSeparation(event);
        
        // Blend with wave-field response based on confidence
        if (validation.confidence > m_config.minWaveFieldThreshold) {
            vec3 waveResponse = computeWaveFieldResponse(event);
            vec3 blendedResponse = lerp(
                separationVector,
                waveResponse,
                validation.confidence
            );
            
            applyHybridResponse(event, blendedResponse);
        } else {
            // Fall back to pure geometric collision
            applyGeometricResponse(event, separationVector);
        }
    }

    float computeHybridConfidence(bool geometricValid,
                                bool fieldValid,
                                const WaveFieldMetrics& metrics) {
        float confidence = 0.0f;
        
        // Base confidence from validation results
        if (geometricValid) confidence += 0.5f;
        if (fieldValid) confidence += 0.3f;
        
        // Adjust based on field metrics
        confidence += metrics.coherence * 0.1f;
        confidence -= std::abs(1.0f - metrics.fieldIntensity) * 0.1f;
        
        // Penalize unstable gradients
        if (metrics.gradientMagnitude > m_config.maxStableGradient) {
            confidence *= 0.8f;
        }
        
        return std::clamp(confidence, 0.0f, 1.0f);
    }

    // Set up shape-maintaining field forces
    void setupShapeIntegrity(const ColliderNode* collider,
                            const ShapeFieldMapping::ShapePattern& pattern) {
        // Create internal field forces that maintain shape
        auto material = collider->getMaterial();
        
        // Stronger coupling for rigid materials
        float fieldCoupling = material.rigidity * m_config.couplingScale;
        
        // Create internal wave patterns that maintain shape
        for (const auto& node : pattern.fieldNodes) {
            createShapeMaintainingWaves(
                node,
                pattern.centerOfField,
                fieldCoupling
            );
        }
    }

    void createShapeMaintainingWaves(const vec3& node,
                                   const vec3& center,
                                   float coupling) {
        // Create waves that maintain relative positions
        WaveProperty maintainerWave;
        maintainerWave.amplitude = coupling;
        maintainerWave.direction = (node - center).normalized();

        m_solver.propagateWave(
            node,
            maintainerWave,
            WaveType::SHAPE_MAINTAINER
        );
    }

    // Update traditional collision shape based on field deformation
    void updateShapeFromField(ColliderNode* collider) {
        auto fieldPattern = getFieldPattern(collider);
        
        // Update shape parameters based on field state
        switch (collider->getShapeType()) {
            case ShapeType::Box: {
                auto& box = static_cast<BoxCollider*>(collider);
                updateBoxDimensions(box, fieldPattern);
                break;
            }
            case ShapeType::Sphere: {
                auto& sphere = static_cast<SphereCollider*>(collider);
                updateSphereRadius(sphere, fieldPattern);
                break;
            }
            case ShapeType::Capsule: {
                auto& capsule = static_cast<CapsuleCollider*>(collider);
                updateCapsuleParameters(capsule, fieldPattern);
                break;
            }
        }
    }

    // Handle traditional shape collision through field interaction
    void handleShapeCollision(ColliderNode* shapeA, ColliderNode* shapeB) {
        // Get field patterns for both shapes
        auto patternA = getFieldPattern(shapeA);
        auto patternB = getFieldPattern(shapeB);

        // Find field intersection regions
        auto intersectionPoints = findFieldIntersections(patternA, patternB);

        for (const auto& point : intersectionPoints) {
            // Create collision waves at intersection points
            CollisionPattern pattern = createCollisionPattern(point, patternA, patternB);
            handleCollision(point, pattern);
        }
    }

    // Apply material properties through field interactions
    void applyMaterialProperties(const vec3& position, const MaterialField& material) {
        // Create property-defining wave patterns
        WaveProperty densityWave = createDensityWave(material.properties.density);
        WaveProperty elasticWave = createElasticityWave(material.properties.elasticity);
        
        // Propagate material property waves
        m_solver.propagatePropertyWaves(position, {densityWave, elasticWave});
    }

    // Update material state based on field interactions
    void updateMaterialState(const vec3& position, MaterialField::StateFields& state) {
        // Sample local field state
        auto fieldState = m_solver.sampleFieldState(position);
        
        // Update stress state
        state.stress.updateStress(
            fieldState.getForce(),
            fieldState.getArea()
        );
        
        // Update thermal state
        state.thermal.temperature = fieldState.getEnergyDensity();
        state.thermal.propagateHeat(m_deltaTime);
        
        // Check for fracture
        if (shouldInitiateFracture(state)) {
            initiateFracture(position, state);
        }
    }

private:
    // Create waves that define material properties
    WaveProperty createDensityWave(const MaterialField::PropertyFields::DensityField& density) {
        WaveProperty wave;
        wave.amplitude = density.baseAmplitude;
        wave.frequency = m_config.densityWaveFrequency;
        wave.phase = computePhaseForDensity(density.localVariation);
        return wave;
    }

    WaveProperty createElasticityWave(
        const MaterialField::PropertyFields::ElasticityField& elasticity) {
        WaveProperty wave;
        wave.amplitude = elasticity.waveSpeed;
        wave.frequency = elasticity.resonanceFreq;
        wave.phase = computePhaseForElasticity(elasticity.dampingFactor);
        return wave;
    }

    // Handle material state changes
    bool shouldInitiateFracture(const MaterialField::StateFields& state) {
        float strainEnergy = computeStrainEnergy(state.stress);
        return strainEnergy > state.fracture.criticalEnergy;
    }

    void initiateFracture(const vec3& position, MaterialField::StateFields& state) {
        // Create fracture waves
        WaveProperty fractureWave;
        fractureWave.amplitude = state.stress.magnitude;
        fractureWave.direction = state.stress.principal;
        
        // Propagate fracture through material
        m_solver.propagateFractureWave(position, fractureWave);
        
        // Update fracture state
        state.fracture.crackTips.push_back(position);
        state.fracture.propagation.push_back(state.stress.principal);
    }

    // Compute derived properties
    float computeStrainEnergy(const MaterialField::StateFields::StressField& stress) {
        // Calculate strain energy density from stress tensor
        return 0.5f * (
            stress.tensor[0][0] * stress.tensor[0][0] +
            stress.tensor[1][1] * stress.tensor[1][1] +
            stress.tensor[2][2] * stress.tensor[2][2]
        ) / m_youngsModulus;
    }
    
    OptimizationSystem m_optimizer;
};

/**
 * @brief ForceFieldConfig defines the configuration for force fields.
 * These are not force fields as their meaning in sci-fi, but rather, they are force fields in the sense that they exert forces on objects.
 */
class ForceFieldConfig {
    /**
     * @brief RangeProfile defines the range and falloff of a force field.
     */
    struct RangeProfile {
        float dissipationRadius;    // Field reach
        float falloffExponent;      // How quickly force decreases
        float baseStrength;         // Initial force magnitude
        
        // Configure for different force types
        static RangeProfile createGravityLike() {
            return { 100.0f, 2.0f, 0.1f }; // Long range, inverse square
        }
        
        // Configure for different force types
        static RangeProfile createContactForce() {
            return { 1.0f, 4.0f, 10.0f };  // Short range, strong
        }
    };

    struct WaveSourcePlacement {
        // Parameters for when placing wave sources on vertices
        float normalOffset;         // Distance along normal
        bool useInternalSources;    // Place inside mesh
        float volumeCoverage;       // Desired volume fill
    };
};

/**
 * @brief FieldGridProcessor handles processing of field grids.
 * It includes algorithms for optimization, compression, and parallelization.
 */
class FieldGridProcessor {
    struct FieldBatch {
        // Dense grid representation for field values
        struct GridBlock {
            static constexpr uint32_t BLOCK_SIZE = 8;
            alignas(32) float values[BLOCK_SIZE][BLOCK_SIZE][BLOCK_SIZE];
            BoundingBox bounds;
            uint32_t resolution;
        };

        // Sparse octree for adaptive resolution
        struct OctreeNode {
            GridBlock block;
            std::array<std::unique_ptr<OctreeNode>, 8> children;
            float importance;  // For adaptive subdivision
        };

        // Compressed field representation
        struct CompressedField {
            // Wavelet compression for low-energy regions
            std::vector<float> coefficients;
            uint32_t compressionLevel;
            
            // Run-length encoding for homogeneous regions
            struct RLEBlock {
                float value;
                uint32_t count;
            };
            std::vector<RLEBlock> rleData;
        };
    };

    void processFieldBlock(GridBlock& block);
};

/**
 * @brief WaveBatchProcessor handles batch processing of waves.
 * Uses SIMD and parallelization for performance.
 */
class WaveBatchProcessor {
    struct WaveBatch {
        // Unique identifier for the batch
        uint32_t id;
        int priority;
        uint64_t timestamp;
        static constexpr uint32_t batchSize = 8;

        // Packed wave properties for SIMD processing
        alignas(32) std::array<float, batchSize> amplitudes;
        alignas(32) std::array<float, batchSize> frequencies;
        alignas(32) std::array<float, batchSize> phases;
        alignas(32) std::array<vec3, batchSize> directions;
        
        // Metadata for batch processing
        uint32_t activeCount;
        uint32_t startIndex;
        
        // Optimization: Pre-calculated trigonometric values
        alignas(32) std::array<float, batchSize> sinValues;
        alignas(32) std::array<float, batchSize> cosValues;
    };

    // Process waves in SIMD-friendly batches
    void processBatch(WaveBatch& batch);
};

/**
 * @brief Refines the field grid adaptively with algorithms based on:
 *  - Energy density
 *  - Energy gradient
 *  - Wave-field boundary
 *  - Samples taken at mesh vertices positions
 */
class AdaptiveGridRefinement {
    /**
     * @brief Refines the grid at a specific vertex.
     */
    void refineAtVertex(const vec3& vertex, const vec3& normal);
};

} // namespace hd



