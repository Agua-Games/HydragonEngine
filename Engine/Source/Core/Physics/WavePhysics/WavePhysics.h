/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePhysics.h
 * @brief WavePhysics is a class which implements a unified physics system, WavePhysics(c), that handles energy transfer and transformation in the engine.
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
 * 
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
 *              Energy (the quantity/data) --> Wave (the carrier pattern: excitation/perturbation/radiation in a field) --> Field (the set/system: energy distribution in space)
 * 
 * - Main elements, entities in the WavePhysics system:
 * 
 *          - Field: Energy distribution in space, be it potential or converted: kinetic/momentum, etc.
 * 
 *          - Dimensional/Spectral Energy: Energy represented as a 4D vector (spectral components), which we can project to scalar when needed for simple calculations, 
 *          rich interaction possibilities between components.
 * 
 *          - Wave: Energy excitation/perturbation/radiation in a field.
 * 
 *          - Wavefront: Wave propagation path. In general we are referring to the Wavefront Tracker: the point in the field where the excitation value of the 
 *          wave/continuum crossed the limiar threshold - manifesting energy enough to be considered a wavefront.
 * 
 *          - Short-lived Particle/Wavefront Tracker: A computational marker for wavefront tracking. In general, referring to "short-lived waves" - waves whose
 *          dissipation is so fast/short lived that it's not worth tracking them any longer. They're meant to contribute to intense and short-lived energy 
 *          transfer in phenomena interesting for realtime games, like impacts, explosions, combustions, heating, transfer of kinectic energy, sound
 *          generated by fluid compression/push, shockwaves, luminous phenomena, ionization (plasma filaments, fields), in various degrees of intensity and 
 *          wavefront sampling density. In the technical side, what we model them to go through is actually physics events like particle decay, pair production, 
 *          change of energy state (energy signature, like "it's a quark", "a photon", "a boson"), change of atomic nuclei/signature, etc.
 * 
 *          - Energy Manifestation: The way energy is expressed in the system. It can be kinetic, thermal, particle, wave (radiation, electromagnetic plus other
 *          known and support for custom/exoteric - 'exoteric' in the sense of the frontier of physics, extreme physics, astrophysics), field.
 * 
 *          - Impact/Collision: Energy transfer between objects. We use these names for easy recognition by users, familiarity, but conceptually we see these
 *          events much more like energy transfer events and also as some sort of temporary entanglement/ressonance/composition, like we see in standing waves, 
 *          wave superposition/composition, quantum physics: their phases interact, ressonate, modulate one another, based on their characteristics.
 * 
 *          - So every time you see the word "impact" or "collision" in the code, it implies actually a wave interference event involving energy transfer.
 *          Like energy rays crossing each other after being emitted: the point where they intertwine will manifest the result of their interaction, in general
 *          manifesting added energy, change of energy state, energy level in that region - like solving their wave-like superposition. It can go from microwave
 *          ressonator devices for game weapons to solving of car crashes, explosions, crumbling of buildings, shock between astrophysical bodies, quasars, etc.
 * 
 *          - Dimension: a set of values seem as  whole. The most defined, "stable", sampled stretches of them can be also seen as states - like in "the states 
 *          of matter", or in particle categories (neutrino, photon, boson, etc).
 * 
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
 * 
 *          - Bonds, Links: we try to model and simplify otherwise complex and ultra-detailed physics interactions, from large scale systems to very small ones
 *          (specially useful for these last ones) using the concept of bonds and links. Strong stable electromagnetic bonds, for instance, or the energy involved,
 *          in "potential form", not released, required to break a stable bond. Matter, solid volumes are some obvious examples. Then shattering, destruction,
 *          fracturing become events which involve and require imense amounts of energy, releasing proportional amounts of energy, in various manifestation types,
 *          until all that energy is spent, finally dissipated and reabsorbed by the environment/system. That means, naturally, automatically, that the most
 *          still, static scenarios are the ones that require the least amount of processing but at the same time the ones which ironically are the most energy
 *          intense - imense amounts of energy is involved to keep them so static, and all that energy is there, waiting to be released.
 * 
 *          - Physics Thread/Physics Update: the physics solving, the solving of so many complex and dynamic elements mean we need to have a dedicated thread,
 *          also leverage multi-threading in it (with proper sync objects, of course). From the perspective of the Physics engine, subsystem, it is indeed the
 *          most fundamental core structure in the engine. All interactions emerge from its existence - the less hardcoded and scripted, the more this is true.
 *          It's as if the gameplay thread is mostly "fake" (in the sense of reflecting the real world), scripted, non-spontaneous: it's most like a "force of will"
 *          factor, a wishlist. While the physics thread and the physics system are much more complex and nuanced, dynamic and emergent. What prevents us from
 *          letting it spread more in realtime is of course performance constraints. And in Hydragon we have the proper harmonic mechanisms to gracefully lead
 *          the possible emergent scenarios to fruitful, interesting ones - e.g. the PatternOrchestrator.
 * 
 *          - *Regarding traditional particle emitters: in WavePhysics, to keep with Energy Conservation, etc, we probably won't introduce emitters, if possible,
 *          as we're trying to stick to a "makes sense", phenomenological approach, considering Energy Conservation Law, Cause-Effect, etc. So, to model an emitter
 *          (arbitrary emission event) we resort to adding a field plus adding "fuel" (energy) to a region of the field. If a proper field is already present we
 *          just add energy to it. This can be useful, for instance, to model magic fx. For more realistic effects, try to simplify cause-effect, energy
 *          conservation, energy transfer, etc. e.g. for a fire effect for a torch, we can sample/query a pre-set thermalEnergy value in the torch prop, adding
 *          also, automatically, a local field, for more resolution.
 * 
 *          - Regarding traditional Rigid Body Physics versus WavePhysics Solid physics: To facilitate modeling collisions, in terms of memory needed, to be on par 
 *          with traditional physics collision approaches in this respect, and at the same time keep the system unified, we see, speaking in a simplistic manner,
 *          rigid body physics (in terms of angular and linear velocities, impulses, etc for a body) as a very sparse field - these velocity vectors being akin to
 *          the vectors in WavePhysics fields. Like the total average momentum for a solid body. Then, what remains for us is to implement the proper conversion from
 *          the many vectors in a solidField to this average, and vice-versa, as needed.      
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

"Based on our discussion, I'd recommend these concrete next steps:

    1. Implement the minimal wave propagation system shown above
    2. Create a simple test scene with a cube falling onto a plane
    3. Develop the visual debugging tools to visualize wave propagation
    4. Implement basic energy conservation checks
    5. Create the wave-to-vector conversion functions for rigid bodies"

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
#include "WavePhysicsSolver.h"

namespace hd {

struct WavePhysicsInfo : public NodeInfo {
    WavePhysicsInfo() {
        NodeType = "Physics/WavePhysics";
        
        inputs = 
            "SolverSettings",  // Settings for the wave solver
            "Optimization",    // Optimization settings
        };
        
        outputs = {
            "SolverState",            // State of the solver
        };        
    }    
};   

class WavePhysics : public Node {
public:
    // === Allocation, Initialization, Loading ===

    explicit WavePhysics(const WavePhysicsInfo& info = WavePhysicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {

    }
    void updateDomainResolution(const OptimizationSystem::DomainOptimizer::LODSystem::LODLevel& level);  
    vec3 getFocusPoint() const;                                 // Usually the camera or player position
    float getDistanceFromFocus(const vec3& focusPoint) const;
    /**
     * @brief SetConstantGravityForce sets a constant global gravity force. It's used as a simplified way to set gravity, as properly modeled gravity 
     * fields would require enormous processing power, for being enormous (big enough to model gradient variations). For lower res approximations
     * (e.g. space sim games, with planet scales far from actual), such gravity fields are feasible, though.
     */
    void setConstantGravityForce(const vec3& gravityDirection, float strength);
    /**
     * @brief SetConstantWindForce sets a constant global wind force. It's used as a simplified wind force approximation in the absence of a proper
     * AtmosphereField in the scene. It applies a reasonably well modeled combination of directional kineticEnergy and momentumPotential to entities
     * in the scene, with some variations akin to how wind behaves in the real world. It will, for instance, make simulated cloth (solidFields with
     * low stiffness areas) flutter and move in the wind as they receive these energy transfers.
     */
    void setConstantWindForce(const vec3& windDirection, float strength, float turbulence, float gustFrequency, float randomness);
    /**
     * @brief EnableEnergyConservation enables or disables the strict keeping of the energy conservation law.
     * This results in the generation of side effects from energy transfer events (like collisions), like debris, sparks, heating of surfaces, 
     * sound punches/shockwaves, all sorts of side effects from collisions which happen as the system tries to solve the proper transfer of energy and 
     * keep the energy conservation law.
     */
    void enableEnergyConservation(bool enable);

    // Events
    void OnEnergyTransfer(const EnergyTransferEvent& event);
    void OnCollision(const CollisionEvent& event);          // This can work both as a convenience method (more familiar to users) and to implement standard RB physics
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WavePhysics() = default;     // Default destructor
}
} // namespace hd