/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Solid.h
 * @brief Solid represents a physics solid body node in the engine's node graph. It is physics-enabled, via WavePhysics.
 * 
 * ARCHITECTURAL NOTES:
 * - Solid supports traditional rigid body joints and constraints via logical links, like ParentLink, PointLink, etc.
 * - It is used to represent any solid shape in the game world, such as boxes, spheres, cylinders, etc.
 * - It also support procedurally generated shapes and input meshes from assets.
 * - It supports interactive features, such as collision detection and response.
 * - It uses the Vulkan API for shape processing.
 * 
 * @todo Unify and consolidate the content in Solid.h and SolidShape.h.
 * @todo Update the whole content to match the latest Object and Node design.
 * @todo Properly integrate with the physics engine (WavePhysics, etc).
 * @todo Create .cpp file and move the implementation there.
 * @todo Organize the existing code into logical sections and functions.
 * @todo Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * @todo Flesh out the class and its methods, structs, enums, etc.
 * @todo After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * @todo Get rid of traditional rigid body dynamics and colliders (after validation), leaving only WavePhysics code.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Transform.h"
#include "WavePhysics.h"
#include "PhysicsTypes.h"             // From WavePhysics, for the body state, collision events, forces, energy state, etc.
#include "PhysicsFields.h"            // From WavePhysics, for the solidField, fluidField, etc.
#include "ProceduralTypes.h"          // From WavePhysics, for the procedural shape types, such as Box, Sphere, Cylinder, etc.
#include "RuntimeVariants.h"        // From our custom usd implementation, optimized for realtime, including LOD support
#include "PhysicsMaterial.h"

namespace hd {

struct SolidInfo : public NodeInfo {
    SolidInfo() {
        NodeType = "Physics/Solid";
        
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

/**
 * @brief Solid represents a physics solid body node in the engine's node graph.
 * It corresponds to a combination of the Collider and RigidBody(+SoftBody) entities in traditional physics engines.
 */
class Solid : public Node {
public:
    // === Structure Definitions ===
    enum class ShapeType {
        Box,
        Sphere,
        Cylinder,
        Capsule,
        Mesh,
        Custom
    };

    /**
     * @brief Energy Transfer Events. Traditionally called "Collisions".
     */
    struct EnergyTransferEvent {
        // Define collision events here
    };

    struct Shape {
        float radius;
        float height;
        vec3 extents;
    };
    
    // Initially, this struct is to allow easy compatibility with, conversion to rigid body dynamics
    struct BodyState {
        vec3 position;                 // Derived from the solidField's momentum potential energy
        vec3 velocity;                 // Derived from the solidField's momentum potential energy
        float mass;                    // Derived from the solidField's nuclear potential energy
        float density;                 // Derived from the solidField's energy density gradient
        PhysicsMaterial material;      // Derived from some solidField's properties/energy values related to material
    };


    // === Allocation, Initialization, Loading ===
    explicit Solid(const SolidInfo& info = SolidInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    SolidField solidField;
    PositionTransform position;
    RotationTransform rotation;
    ScaleTransform scale;
    vec3 velocity;
    float nucleusPotential;
    float mass;                         // Derived directly from nucleusPotential, kept only to help users not used to WavePhysics. May be removed later.
    ShapeType shapeType;
    Shape shape;
    float meshShapeCaptureStride;       // Set the vertex stride for mesh shape capture, for simpler collision/energy transfer shapes
    float radius = 0.0f;
    vec3 extents = vec3(1.0f);

    // We set default physics material proper for the case the Solid has no mesh assigned
    std::vector<PhysicsMaterial> materials;
    materials[0]::friction = 0.0f;
    materials[0]::restitution = 0.0f;
    materials[0]::density = 0.0f;

    std::vector<vec3> vertices;
    BodyState bodyState;
    bool isDynamic = false;

    // === Processing ===
    void processNode() override {
        // Process inputs
        auto shape = getInputValue<Shape>("Shape");
        auto mass = getInputValue<float>("Mass");
        auto density = getInputValue<float>("Density");
        auto material = getInputValue<PhysicsMaterial>("Material");        // To override the meshes physics material, normally tagged in the mesh
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
    void getMaterialsFromMesh();                // Can apply different physics materials from different parts of the mesh using assigned mesh materials and their tags
    void setDynamic(bool isDynamic);
    void setShape(const Shape& shape);
    void setMaterial(const PhysicsMaterial& material);
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
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Solid() = default;     // Default destructor

private:
    // === Processing ===
    // Helper functions
    EnergyTransferEvents processEnergyTransfer(const BodyState& bodyState);
    EnergyTransferEvents processCollisions(const BodyState& bodyState);         // Specific to solid-solid collisions. Used for rigid body dynamics (compatibility)
    Forces applyForces(const BodyState& bodyState);
    void updateBodyState(BodyState& bodyState, const Forces& forces);
    EnergyState computeEnergyState(const BodyState& bodyState);
};

} // namespace hd

