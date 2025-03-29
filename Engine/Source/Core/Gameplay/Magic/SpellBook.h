/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpellBook.h
 * @brief Header file for the SpellBook class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpellBook is a class that represents a spell book in Hydragon.
 * - It is used to represent any spell book in the game world, such as books, scrolls, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other spell books, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Prop.h"
#include "Spell.h"
#include "DataTable.h"
#include "Ability.h"
#include "MemoryBank.h"

namespace hd {

struct SpellBookInfo : public PropInfo {
    SpellBookInfo() {
        nodeType = "Gameplay/SpellBook";
        
        inputs = {
            "spellBookData",   // Spell book data
            "environment",     // Environment data
            "characterData",   // Character data
            "spellBookState"   // Spell book state
        };
        
        outputs = {
            "spellBookStatus", // Spell book status
            "spellBookMetrics" // Spell book performance metrics
        };
    }
};

class SpellBook : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpellBook(const SpellBookInfo& info = SpellBookInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===

    void processNode() override { }
    void addSpell(const std::string& spellName);
    void removeSpell(const std::string& spellName);
    void cast();
    void target();
    void cooldown();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpellBook() = default;     // Default destructor
};

} // namespace hd
