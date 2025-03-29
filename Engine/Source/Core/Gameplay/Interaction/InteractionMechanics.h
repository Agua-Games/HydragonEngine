/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InteractionMechanics.h
 * @brief Header file for the InteractionMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - InteractionMechanics is a class that represents interaction mechanics in Hydragon.
 * - It is used to represent any interaction mechanics in the game world, such as player interactions, character interactions, etc.
 * - It supports interactive features, such as interacting, communicating, and socializing. Also supports two-way messaging with other interaction mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include "Math.h"
#include "Ability.h"
#include "InteractionManager.h"
#include "InteractionEvent.h"
#include "Interaction.h"

namespace hd {

class InteractionMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit InteractionMechanics(const InteractionMechanicsInfo& info = InteractionMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InteractionMechanics() = default;     // Default destructor
};

} // namespace hd

