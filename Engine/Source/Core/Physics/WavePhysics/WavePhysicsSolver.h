/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WavePhysicsSolver.h
 * @brief Header file for the WavePhysicsSolver class.
 * 
 * ARCHITECTURAL NOTES:
 * - WavePhysicsSolver is a class that represents a unified physics solver in Hydragon.
 * - It is used to represent physics solvers based on our approach, WavePhysics, which tries to simplify physics in games by unifying traditionally separate physics
 * libraries, like rigid body physics, fluid physics, particle physics, etc.
 * - At first, our focus is on entertainment uses, approximations, etc, but we'll try hard to make it in a way that it may be used later for more scientifically
 * strict uses, to avoid having to rework it from scratch, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "<unordered_map>"
#include "WavePhysics.h"


/**
 * @brief Solver for wave physics.
 * Internal solver class that handles actual physics calculations. WavePhysicsSolver is meant to be instantiated modularly, managed by WavePhysics, and allow for
 * different solver configurations, from the heavier global solver to the more lightweight local, specialized, effects-oriented solvers.
 * We could have made it into a node, but we opted for an internal class to facilitate user workflow, communications between solvers - they're handled internally,
 * and directly, so we have the best of both worlds, as we can turn on and off connections between solvers, and have them communicate directly.
 */
class WavePhysicsSolver {
    public:
        // === Allocation, Initialization, Loading ===
        WavePhysicsSolver(const SolverConfig& config) {
            // Initialize solver with configuration
        }

        std::unordered_map<SolverId, std::unique_ptr<WavePhysicsSolver>> m_solvers;
        SolverId m_nextSolverId = 0;

        // TODO: 
        // - Implement the solver
        // - Allow, first of all, for different configurations, based on probable workflows, uses, and user preference. For this user will resort to Vulkan-style
        // configuration info structs, and the implementation details themselves are handled internally.
        // - Methods, bridges to connect and disconnected transfer of data between solvers, safely.
        
        // === Processing ===
        void processWaves(float deltaTime);
        void resolveFieldInteractions(float deltaTime);
        void updateEnergyStates(float deltaTime);

        // === Cleanup ===
        ~WavePhysicsSolver() {
            // Cleanup resources
        }
};