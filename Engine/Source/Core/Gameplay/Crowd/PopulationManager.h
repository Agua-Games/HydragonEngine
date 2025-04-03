/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PopulationManager.h
 * @brief Header file for the PopulationManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - PopulationManager is a singleton class that manages the population in the game.
 * - It is responsible for managing the population in the game.
 * - It supports interactive features, such as population management, population communication, population objectives, etc. Also supports two-way messaging with other population managers, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include "PopulationMechanics.h"
#include "Population.h"

namespace hd {

/**
 * @class PopulationManager
 * @brief Class representing population manager in Hydragon.
 */
class PopulationManager : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit PopulationManager(const PopulationManagerInfo& info = PopulationManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable populationManagerData;
    PopulationMechanics populationMechanics;
    std::vector<Population> populations;

    // === Processing ===
    void processNode() override {}
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~PopulationManager() = default;     // Default destructor
};

} // namespace hd