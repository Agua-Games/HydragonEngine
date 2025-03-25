/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MaterialEvolution.h
 * @brief Header file for the MaterialEvolution class.
 * 
 * ARCHITECTURAL NOTES:
 * - MaterialEvolution is a class that represents a material evolution node in the engine's node graph.
 * - It is used to evolve materials over time, using genetic algorithms, based on fitness and constraints.
 * - It can be used to create dynamic and evolving materials, such as procedural textures and shaders.
 * 
 */
#pragma once
#include "Node.h"
#include "Material.h"

namespace hd {

struct MaterialEvolutionInfo : public NodeInfo {
    MaterialEvolutionInfo() {
        NodeType = "Rendering/MaterialEvolution";
        inputs = {
            "InitialMaterial",  // Initial material to evolve
            "FitnessFunction",  // Fitness function to evaluate materials
            "Constraints",      // Constraints for material evolution
            "MutationRate",     // Mutation rate for genetic algorithm
            "PopulationSize",   // Population size for genetic algorithm
            "Generations",      // Number of generations to evolve
            "Seed"              // Random seed for genetic algorithm
        };
        outputs = {
            "EvolvedMaterial"   // Evolved material
        };
    }
};

class MaterialEvolution : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MaterialEvolution(const MaterialEvolutionInfo& info = MaterialEvolutionInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing === 
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MaterialEvolution() = default;
};

} // namespace hd