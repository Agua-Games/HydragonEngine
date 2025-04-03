/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Population.h
 * @brief Header file for the Population class.
 * 
 * ARCHITECTURAL NOTES:
 * - Population is a class that represents a population in Hydragon.
 * - It is used to represent any population in the game world, such as player populations, enemy populations, etc.
 * - It supports interactive features, such as population management, population communication, population objectives, etc. Also supports two-way messaging with other populations, environment, character, etc.
 */
#pragma once
#include "Core.h"
#include "Node.h"
#include "DataTable.h"
#include "PopulationMechanics.h"

namespace hd {

/**
 * @class Population
 * @brief Class representing a population in Hydragon.
 */
class Population : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Population(const PopulationInfo& info = PopulationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values

    // Set default values
    DataTable populationData;    // Population data, such as population size, population density, population diversity, etc.
    PopulationMechanics populationMechanics;    // Population mechanics, such as population growth, population decay, population development, etc.

    // === Processing ===
    void processNode() override {}
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Population() = default;     // Default destructor
};

} // namespace hd
