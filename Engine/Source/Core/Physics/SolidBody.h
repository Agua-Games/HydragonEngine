/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SolidBody.h
 * @brief SolidBody represents a solid body node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Solid body nodes are used to represent and process solid bodies in physics simulations.
 * 
 * @todo Update the whole content to match the latest Object and Node design.
 * @todo Properly integrate with the physics engine (WavePhysics, etc).
 * @todo Create .cpp file and move the implementation there.
 * @todo Organize the existing code into logical sections and functions.
 * @todo Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * @todo Flesh out the class and its methods, structs, enums, etc.
 * @todo After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

#include "Node.h"
//#include "PhysicsTypes.h"
#include "WavePhysics.h"

namespace hd {

struct SolidBodyInfo : public NodeInfo {
    SolidBodyInfo() {
        NodeType = "Physics/SolidBody";
        
        Inputs = {
            "Shape",            // Solid body shape
            "Mass",            // Body mass
            "Density",         // Body density
            "Material",        // Material properties
            "InitialPosition", // Initial position
            "InitialVelocity"  // Initial velocity
        };
        
        Outputs = {
            "BodyState",       // Current body state
            "CollisionEvents", // Collision events
            "Forces",          // Applied forces
            "EnergyState"      // Energy state
        };
    }
};

class SolidBody : public Node<BodyState, CollisionEvents, Forces, EnergyState> {
public:
    explicit SolidBody(const SolidBodyInfo& info = SolidBodyInfo())
        : Node(info) {}

    void processNodeGraph() override {
        // Process inputs
        auto shape = getInputValue<Shape>("Shape");
        auto mass = getInputValue<float>("Mass");
        auto density = getInputValue<float>("Density");
        auto material = getInputValue<Material>("Material");
        auto initialPosition = getInputValue<vec3>("InitialPosition");
        auto initialVelocity = getInputValue<vec3>("InitialVelocity");

        // Initialize body state
        BodyState bodyState;
        bodyState.position = initialPosition;
        bodyState.velocity = initialVelocity;
        bodyState.mass = mass;
        bodyState.density = density;
        bodyState.material = material;

        // Process collisions and forces
        CollisionEvents collisionEvents = processCollisions(bodyState);
        Forces forces = applyForces(bodyState);

        // Update body state
        updateBodyState(bodyState, forces);

        // Output results
        setOutputValue("BodyState", bodyState);
        setOutputValue("CollisionEvents", collisionEvents);
        setOutputValue("Forces", forces);
        setOutputValue("EnergyState", computeEnergyState(bodyState));
    }

private:
    CollisionEvents processCollisions(const BodyState& bodyState);
    Forces applyForces(const BodyState& bodyState);
    void updateBodyState(BodyState& bodyState, const Forces& forces);
    EnergyState computeEnergyState(const BodyState& bodyState);
};

} // namespace hd

