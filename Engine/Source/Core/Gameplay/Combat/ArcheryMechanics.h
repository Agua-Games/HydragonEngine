/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ArcheryMechanics.h
 * @brief Header file for the ArcheryMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - ArcheryMechanics is a class that represents archery mechanics in Hydragon.
 * - It is used to represent any archery mechanics in the game world, such as accuracy, stability, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other archery mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Archery.h"

namespace hd {

/**
 * @class ArcheryMechanics
 * @brief Class representing archery mechanics in Hydragon.
 */
class ArcheryMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ArcheryMechanics(const ArcheryMechanicsInfo& info = ArcheryMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addArchery(const std::string& archeryName);
    void removeArchery(const std::string& archeryName);
    void processArchery();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ArcheryMechanics() = default;     // Default destructor
};

} // namespace hd

