/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Cigar.h
 * @brief Header file for the Cigar class.
 * 
 * ARCHITECTURAL NOTES:
 * - Cigar is a class that represents a cigar in Hydragon.
 * - It is used to represent any cigar in the game world, such as cigars, cigarettes, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other cigars, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct CigarInfo : public EquipmentInfo {
    CigarInfo() {
        nodeType = "Gameplay/Cigar";
        
        inputs = {
            "cigarType",  // Type of cigar (cigar, cigarette, etc.)
            "cigarData",  // Cigar data
            "environment", // Environment data
            "characterData", // Character data
            "cigarState" // Cigar state
        };
        
        outputs = {
            "cigarStatus", // Cigar status
            "cigarMetrics" // Cigar performance metrics
        };
    }
};

class Cigar : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Cigar(const CigarInfo& info = CigarInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void equip();
    void unequip();
    void upgrade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Cigar() = default;     // Default destructor
};

} // namespace hd
