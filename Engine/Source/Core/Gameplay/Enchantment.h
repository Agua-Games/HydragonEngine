/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Enchantment.h
 * @brief Header file for the Enchantment class.
 * 
 * ARCHITECTURAL NOTES:
 * - Enchantment is a class that represents an enchantment in Hydragon.
 * - It is used to represent any enchantment in the game world, such as magic, blessings, curses, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other enchantments, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct EnchantmentInfo : public NodeInfo {
    EnchantmentInfo() {
        nodeType = "Gameplay/Enchantment";
        
        inputs = {
            "enchantmentType",  // Type of enchantment (magic, blessing, curse, etc.)
            "enchantmentData",  // Enchantment data
            "environment",      // Environment data
            "characterData",    // Character data
            "enchantmentState"  // Enchantment state
        };
        
        outputs = {
            "enchantmentStatus",  // Enchantment status
            "enchantmentMetrics"  // Enchantment performance metrics
        };
    }
};

class Enchantment : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Enchantment(const EnchantmentInfo& info = EnchantmentInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::unordered_map<std::string, float> effects;
    std::unordered_map<std::string, float> cooldowns;   // Cooldowns for each effect

    // === Processing ===
    void processNode() override { }
    void addEffect(const std::string& effectName, float effectValue);
    void removeEffect(const std::string& effectName);
    void cast();
    void target();
    void cooldown();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Enchantment() = default;     // Default destructor
};

} // namespace hd
