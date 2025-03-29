/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MagicMechanics.h
 * @brief Header file for the MagicMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - MagicMechanics is a class that represents magic mechanics in Hydragon.
 * - It is used to represent any magic mechanics in the game world, such as spells, powers, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other magic mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"

namespace hd {

struct MagicMechanicsInfo : public NodeInfo {
    MagicMechanicsInfo() {
        nodeType = "Gameplay/MagicMechanics";
        
        inputs = {
            "magicMechanicsData",// Magic mechanics data
            "environment",       // Environment data
            "characterData",     // Character data
            "magicMechanicsState"// Magic mechanics state
        };
        
        outputs = {
            "magicMechanicsStatus",// Magic mechanics status
            "magicMechanicsMetrics"// Magic mechanics performance metrics
        };
    }
};

class MagicMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit MagicMechanics(const MagicMechanicsInfo& info = MagicMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float regeneration = 0.0f;
    float maxMana = 0.0f;
    DataTable magicMechanicsData;

    // === Processing ===
    void processNode() override { }
    void processMagicMechanics();
    void addEnchantment(const std::string& enchantmentName);
    void removeEnchantment(const std::string& enchantmentName);
    void addRune(const std::string& runeName);
    void removeRune(const std::string& runeName);
    void addSpell(const std::string& spellName);
    void removeSpell(const std::string& spellName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MagicMechanics() = default;     // Default destructor
};

} // namespace hd
