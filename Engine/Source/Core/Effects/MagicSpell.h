/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MagicSpell.h
 * @brief Header file for the MagicSpell class.
 * 
 * ARCHITECTURAL NOTES:
 * - MagicSpell is a class that represents a magic spell in the engine's node graph.
 * - It is used to represent any magic spell in the game world, such as spells, powers, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other spells, environment, character, etc.
 * - It makes use of WavePhysics, MagicField, for spell effects which support energy transfer and propagation - thermal, kinetic, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Effect.h"

namespace hd {

enum class MagicSpellType {
    Spell,
    Power,
    Custom
};

struct MagicSpellInfo : public EffectInfo {
    MagicSpellInfo() {
        nodeType = "Effects/MagicSpell";
        
        inputs = {
            "type",            // Type of magic spell (spell, power, etc.)
            "damage",          // Damage dealt by the magic spell
            "cooldown",        // Cooldown time of the magic spell
            "range",           // Range of the magic spell (melee, ranged, etc.)
            "cost",            // Cost of the magic spell (mana, energy, etc.)
            "targeting",       // Targeting type of the magic spell (single, area, etc.)
            "tickRate",        // Tick rate of the magic spell (for continuous effects)
            "magicField",      // Magic field data (stats, etc.)
            "affectMask",      // Affect mask (stats, etc.)
            "magicSpellData",  // Magic spell data (stats, etc.)
            "environment",     // Environment data (stats, etc.)
            "characterData",   // Character data (stats, etc.)
            "magicSpellState"  // Magic spell state (stats, etc.)
        };
        
        outputs = {
            "magicSpellStatus", // Magic spell status (stats, etc.)
            "magicSpellMetrics", // Magic spell performance metrics
            "magicField"        // Magic field data (stats, etc.)
        };
    }
};

class MagicSpell : public Effect {
public:
    // === Allocation, Initialization, Loading ===
    explicit MagicSpell(const MagicSpellInfo& info = MagicSpellInfo())
        : Effect(info) {}
    void initialize() override {}
    void load() override {}

    // Set default values
    MagicSpellType type = MagicSpellType::Spell;
    float damage = 0.0f;
    float cooldown = 0.0f;
    float range = 0.0f;
    float radius = 0.0f;
    float cost = 0.0f;
    float force = 0.0f;
    float momentum = 0.0f;
    float duration = 0.0f;
    float healing = 0.0f;
    float freezeChance = 0.0f;
    float protection = 0.0f;
    std::string targeting = "single";
    float tickRate = 0.0f;
    DataTable magicField = DataTable();
    DataTable affectMask = DataTable();
    DataTable magicSpellData = DataTable();
    DataTable environment = DataTable();
    DataTable characterData = DataTable();
    DataTable magicSpellState = DataTable();

    // === Processing ===
    void processNode() override {
        type = getInputValue<MagicSpellType>("type");
        damage = getInputValue<float>("damage");
        cooldown = getInputValue<float>("cooldown");
        range = getInputValue<float>("range");
        cost = getInputValue<float>("cost");
        targeting = getInputValue<std::string>("targeting");
        tickRate = getInputValue<float>("tickRate");
        magicField = getInputValue<DataTable>("magicField");
        affectMask = getInputValue<DataTable>("affectMask");
        magicSpellData = getInputValue<DataTable>("magicSpellData");
        environment = getInputValue<DataTable>("environment");
        characterData = getInputValue<DataTable>("characterData");
        magicSpellState = getInputValue<DataTable>("magicSpellState");

        // Process magic spell
        auto magicSpellStatus = updateMagicSpell(type, damage, cooldown, range, cost, targeting, tickRate, magicField, affectMask, magicSpellData, environment, characterData, magicSpellState);

        // Set outputs
        setOutputValue("magicSpellStatus", magicSpellStatus);
        setOutputValue("magicSpellMetrics", computeMagicSpellMetrics(magicSpellStatus));
        setOutputValue("magicField", computeMagicField(magicSpellStatus));
    }
    void update() override {}

    // Magic spell processing
    DataTable updateMagicSpell(const MagicSpellType& type, const float& damage, const float& cooldown, const float& range, const float& cost, const std::string& targeting, const float& tickRate, const DataTable& magicField, const DataTable& affectMask, const DataTable& magicSpellData, const DataTable& environment, const DataTable& characterData, const DataTable& magicSpellState);
    DataTable computeMagicSpellMetrics(const DataTable& magicSpellStatus);
    DataTable computeMagicField(const DataTable& magicSpellStatus);

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MagicSpell() = default;     // Default destructor
};

} // namespace hd
