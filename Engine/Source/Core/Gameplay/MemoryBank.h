/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MemoryBank.h
 * @brief Header file for the MemoryBank class.
 * 
 * ARCHITECTURAL NOTES:
 * - MemoryBank is a class that represents a memory bank in the engine's node graph.
 * - It is used to load and manage memory banks, which are collections of memory files, from character memory to environment memory, credits, archives, etc.
 * - It is compatible with FMOD banks.
 * - It uses the FMOD library for memory bank management.
 * 
 */
#pragma once
//#include <fmod.hpp>           // Commented out until we properly include FMOD
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"

namespace hd {

struct MemoryBankInfo : public NodeInfo {
    MemoryBankInfo() {
        nodeType = "Gameplay/MemoryBank";
        
        inputs = {
            "memoryBankFile",        // Memory bank file
            "crossfadeTime",        // Crossfade time for memory bank transitions
            "memoryBankParams"       // Memory bank parameters
        };
        
        outputs = {
            "memoryBankStatus",      // Memory bank status
            "memoryBankMetrics"      // Memory bank performance metrics
        };
    }
};

class MemoryBank : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit MemoryBank(const MemoryBankInfo& info = MemoryBankInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}
    unload() override {}

    // Set default values
    std::unordered_map<std::string, AudioFile*> memoryFiles;
    float crossfadeTime = 0.0f;
    std::unordered_map<std::string, std::any> memoryBankParams;

    // === Processing ===
    void processNode() override {
        memoryFiles = getInputValue<std::unordered_map<std::string, AudioFile*>>("memoryFiles");
        crossfadeTime = getInputValue<float>("crossfadeTime");
        memoryBankParams = getInputValue<std::unordered_map<std::string, std::any>>("memoryBankParams");
    }
    void update();

    // === Cleanup ===
    void cleanup() override {}
    ~MemoryBank() = default;     // Default destructor
};

} // namespace hd

