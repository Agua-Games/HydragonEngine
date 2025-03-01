/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralCore.h
 * @brief ProceduralCore is the core class that manages the procedural generation process.
 * 
 * ARCHITECTURAL NOTES:
 * - The end goal of this structure is to allow, among other things, the engine to self-replicate,
 *   self-modify, procedurally generate and modify nodes themselves.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"

namespace hd {

class ProceduralCore {
public:
    struct GenerationState {
        uint64_t masterSeed;
        std::vector<OctaveParams> harmonics;
        HarmonyParams systemBalance;
        std::map<SystemDomain, float> domainWeights;
    };

    // Core procedural interface
    void SetState(const GenerationState& state);
    void RegenerateFromState();
    
    // System modification
    void ModifyHarmonics(const std::vector<OctaveParams>& newHarmonics);
    void AdjustSystemBalance(const HarmonyParams& newBalance);
    
    // Validation and safety
    bool ValidateState(const GenerationState& state) const;
    bool CanReachState(const GenerationState& targetState) const;
    
private:
    // Orchestration of procedural systems
    HD_ProceduralOrchestrator& m_orchestrator;
    
    // State tracking
    GenerationState m_currentState;
    std::vector<GenerationState> m_stateHistory;
    
    // System domains
    std::unordered_map<SystemDomain, std::unique_ptr<IProceduralSystem>> m_systems;
};

// Example of a procedural system that can modify itself
class SelfModifyingProceduralSystem : public IProceduralSystem {
public:
    void Generate() override {
        // Generate base content
        auto basePattern = GenerateBasePattern();
        
        // Allow system to modify its own parameters
        auto evolution = CreateEvolutionNode();
        evolution->ConnectInput("SourcePattern", basePattern);
        
        // System can modify its own generation rules
        auto modifier = CreateModifierNode();
        modifier->ConnectInput("CurrentRules", GetGenerationRules());
        
        // Apply and validate modifications
        if (ValidateModifications(modifier->GetOutput("ModifiedRules"))) {
            ApplyNewRules(modifier->GetOutput("ModifiedRules"));
        }
    }
};

} // namespace hd