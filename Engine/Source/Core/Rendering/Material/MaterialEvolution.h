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
#include <functional>
#include <vector>
#include "Node.h"
#include "Material.h"
#include "MaterialTypes.h"

namespace hd {

struct MaterialEvolutionInfo : public NodeInfo {
    MaterialEvolutionInfo() {
        nodeType = "Rendering/MaterialEvolution";
        inputs = {
            "initialMaterial",  // Initial material to evolve
            "fitnessFunction",  // Fitness function to evaluate materials
            "constraints",      // Constraints for material evolution
            "mutationRate",     // Mutation rate for genetic algorithm
            "populationSize",   // Population size for genetic algorithm
            "generations",      // Number of generations to evolve
            "seed"              // Random seed for genetic algorithm
        };
        outputs = {
            "evolvedMaterial"   // Evolved material
        };
    }
};

class MaterialEvolution : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MaterialEvolution(const MaterialEvolutionInfo& info = MaterialEvolutionInfo())
        : Node(info) {}
      
    // Defaults
    FitnessFunction fitnessFunction = nullptr;
    std::vector<Constraint> constraints = {};
    MutationType mutationStrategy = MutationType::Random;
    float mutationRate = 0.1f;
    int populationSize = 100;
    int generations = 50;
    int seed = 42;

    initialize() override {}
    load() override {}

    // === Processing === 
    void processNode() override {
        fitnessFunction = getInputValue<FitnessFunction>("fitnessFunction");
        constraints = getInputValue<std::vector<Constraint>>("constraints");
        mutationRate = getInputValue<float>("mutationRate");
        mutationStrategy = getInputValue<MutationType>("mutationStrategy");
        populationSize = getInputValue<int>("populationSize");
        generations = getInputValue<int>("generations");
        seed = getInputValue<int>("seed");
    }

    void adapt();
    void evolve();
    void mutate();     // Mutation function for genetic algorithm
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MaterialEvolution() = default;
};

} // namespace hd