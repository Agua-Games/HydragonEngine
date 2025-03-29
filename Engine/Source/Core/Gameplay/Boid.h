/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Boid.h
 * @brief The Boid class is used for very simple creature modeling. But is has some functionality for AI, flocking, navigation, and flight.
 * It's useful for filling the environment with life, ambient creatures. For this purpose, it is very optimized and parallel processed, leveraging
 * streamlined tensors, gpu Compute, etc. Swarms of insects, worms, ambient distant birds, schools of fish, etc.
 * 
 * These examples demonstrate:
 * 
 *  - Boid flocking behavior with separation, alignment, and cohesion
 *  - Obstacle avoidance with collision detection
 *  - Path following with spline interpolation
 *  - Predator-prey simulation with pursuit and evasion
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Creature.h"

namespace hd {

struct BoidInfo : public NodeInfo {
    BoidInfo() {
        nodeType = "Gameplay/Boid";
        
        inputs = {
            "position",        // Current position of the boid
            "velocity",        // Current velocity of the boid
            "acceleration",    // Current acceleration of the boid
            "neighbors",       // List of neighboring boids
            "obstacles",       // List of obstacles in the environment
            "path",            // Path to follow (if any)
            "predators",       // List of predators in the environment
            "prey",            // List of prey in the environment
            "flockSize",       // Size of the flock
            "maxSpeed",        // Maximum speed of the boid
            "maxForce",        // Maximum force applied to the boid
            "separationWeight",    // Weight for separation behavior
            "alignmentWeight",     // Weight for alignment behavior
            "cohesionWeight",      // Weight for cohesion behavior
            "avoidanceWeight",     // Weight for obstacle avoidance
            "pathFollowingWeight", // Weight for path following (if any)
            "pursuitWeight",       // Weight for predator pursuit (if any)
            "evasionWeight"        // Weight for prey evasion (if any)
        };
        
        outputs = {
            "newPosition",  // New position of the boid
            "newVelocity",  // New velocity of the boid
            "newAcceleration" // New acceleration of the boid
        };
    }
};

class Boid : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Boid(const BoidInfo& info = BoidInfo())
        : Creature(info) {}
    initialize() override {}
    load() override {} 

    // === Processing ===
    void processNode() override { }
    void flock();
    void avoidObstacles();
    void followPath();
    void pursuePredators();
    void evadePrey();
    void applyBehaviors();
    void applyForces();
    void limitSpeed();
    void limitForce();
    void updatePosition();
    void updateVelocity();
    void updateAcceleration();
    void updateNeighbors();
    void updateObstacles();
    void updatePath();
    void updatePredators();
    void updatePrey();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Boid() = default;     // Default destructor
};

} // namespace hd

