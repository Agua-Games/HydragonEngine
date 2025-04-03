/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CrowdMechanics.h
 * @brief Header file for the CrowdMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CrowdMechanics is a class that represents crowd mechanics in Hydragon.
 * - It is used to represent any crowd mechanics in the game world, such as mobs, crowds, etc.
 * - It supports interactive features, such as flocking, schooling, and swarming. Also supports two-way messaging with other crowds, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

/**
 * @class CrowdMechanics
 * @brief Class representing crowd mechanics in Hydragon.
 */
class CrowdMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CrowdMechanics(const CrowdMechanicsInfo& info = CrowdMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float density = 0.0f;
    float enthusiasm = 0.0f;
    DataTable crowdMechanicsData;

    // === Processing ===
    void processNode() override { }
    void addCrowd(const std::string& crowdName);
    void removeCrowd(const std::string& crowdName);
    void processCrowd();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CrowdMechanics() = default;     // Default destructor
};

} // namespace hd

