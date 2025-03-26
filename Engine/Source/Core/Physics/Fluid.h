/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Fluid.h
 * @brief represents a physics fluid body node in the engine's node graph. It is physics-enabled, via WavePhysics.
 * 
 * ARCHITECTURAL NOTES:
 * - Fluid holds one or more fluid fields and supports fluid simulation and seamless interaction with other physics entities, like particles, solids, etc.
 * - It is used  in general to represent fields such as atmosphere, EM field, fire, plasma, gas, liquid, within a shape/container in the game world, 
 * such as boxes, spheres, cylinders, custom meshes, etc.
 * - It supports procedurally generated shapes and input meshes from assets.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "WavePhysics.h"        // From WavePhysics, for the fluidField, etc.
#include "PhysicsFields.h"      // From WavePhysics, for the solidField, fluidField, etc.
#include "ProceduralTypes.h"    // From WavePhysics, for the procedural shape types, such as Box, Sphere, Cylinder, etc.

namespace hd {

struct FluidInfo : public NodeInfo {
    FluidInfo() {
        NodeType = "Physics/Fluid";
        
        inputs = {
            "ShapeType",       // Type of the solid shape (Box, Sphere, Cylinder, Mesh, Custom)
            "Shape",           // Solid body shape
            "Extents",         // Dimensions of the shape
            "Mass",            // Body mass
            "Density",         // Body density
            "Material",        // Material properties
            "Position",        // Initial position
            "Velocity",        // Initial velocity
            "Dynamic"          // Whether the body is dynamic or static 
            "Material",        // To override the meshes physics material, normally tagged in the mesh
        };
        
        outputs = {
            "CollisionEvents", // Collision events
            // Add more outputs as needed
        };
    }
};

class Fluid : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Fluid(const FluidInfo& info = FluidInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void setDynamic(bool isDynamic);
    void setShape(const Shape& shape);
    void setMaterial(const Material& material);
    void setInitialPosition(const vec3& position);
    void setInitialVelocity(const vec3& velocity);
    void setDensity(float density);
    void setNucleusPotential(float nucleusPotential);
    void addMomentumPotential(const vec3& momentum);
    void processEnergyTransfer();

    // These are for compatibility with traditional physics engines.
    void setMass(float mass);
    void addForce(const vec3& force);                   // We derive forces directly from energy fields. Mostly for compatibility with traditional physics engines.
    void processCollisions();

    void processPhysics();

    void () override {
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
        std::vector<EnergyTransferEvent> energyTransferEvents = processEnergyTransfer(bodyState);
        std::vector<EnergyTransferEvent> collisionEvents = processCollisions(bodyState);
        Forces forces = applyForces(bodyState);

        // Update body state
        updateBodyState(bodyState, forces);

        // Output results
        setOutputValue("BodyState", bodyState);
        setOutputValue("CollisionEvents", collisionEvents);
        setOutputValue("Forces", forces);
        setOutputValue("EnergyState", computeEnergyState(bodyState));
    }

    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Fluid() = default;     // Default destructor
};

} // namespace hd
