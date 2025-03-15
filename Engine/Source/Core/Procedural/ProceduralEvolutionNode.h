/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProceduralEvolutionNode.h
 * @brief EvolutionNode represents an evolution node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Evolution nodes are used to evolve patterns over time, using genetic algorithms, based on fitness and constraints.
 * - They can be used to create dynamic and evolving patterns, such as clouds, trees, and other natural phenomena.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Core/NodeGraph/Node.h"

namespace hd {

struct HD_EvolutionInfo : public NodeInfo {
    HD_EvolutionInfo() {
        NodeType = "Procedural/Evolution";
        
        Inputs = {
            "SourcePattern",
            "TimeScale",
            "EvolutionRules",
            "Constraints"
        };
        
        Outputs = {
            "EvolvedPattern",
            "EvolutionState",
            "TransitionMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_ProceduralEvolutionNode : public Node {
public:
    explicit HD_ProceduralEvolutionNode(const HD_EvolutionInfo& info = HD_EvolutionInfo())
        : Node(info) {
        // Register with orchestrator on creation
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        m_evolutionPatternId = orchestrator.RegisterPattern(
            std::make_unique<ProceduralPattern>(
                ProceduralPatternType::Evolution,
                CreateInitialEvolutionParams()
            )
        );
    }

    ~HD_ProceduralEvolutionNode() {
        // Cleanup registration
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        orchestrator.UnregisterPattern(m_evolutionPatternId);
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Get inputs
        auto sourcePattern = GetInputValue<ProceduralPatternData>("SourcePattern");
        auto evolutionRules = GetInputValue<OctaveParams>("EvolutionRules");
        auto constraints = GetInputValue<HarmonyParams>("Constraints");
        
        // Initialize evolution if needed
        if (!m_evolutionState.currentGeneration) {
            InitializeEvolution(sourcePattern);
        }

        // Get orchestrator's fine-tuning parameters
        auto orchestratorPattern = orchestrator.GetProceduralPattern(m_evolutionPatternId);
        UpdateEvolutionParams(orchestratorPattern);

        // Evolve pattern with orchestrator guidance
        EvolvePattern(evolutionRules, constraints);
        
        // Update orchestrator about evolution progress
        NotifyOrchestrator();
        
        // Set outputs
        SetOutputValue("EvolvedPattern", m_evolutionState.bestPattern);
        SetOutputValue("EvolutionState", GetEvolutionMetrics());
        SetOutputValue("TransitionMetrics", CalculateTransitionMetrics());
    }

private:
    EvolutionState m_evolutionState;
    EvolutionParameters m_params;
    std::string m_evolutionPatternId;

    void UpdateEvolutionParams(const ProceduralPatternData& orchestratorPattern) {
        // Update evolution parameters based on orchestrator's guidance
        if (orchestratorPattern.parameters.contains("mutationRate")) {
            m_params.mutationRate = orchestratorPattern.parameters.at("mutationRate");
        }
        if (orchestratorPattern.parameters.contains("crossoverRate")) {
            m_params.crossoverRate = orchestratorPattern.parameters.at("crossoverRate");
        }
        if (orchestratorPattern.parameters.contains("adaptationRate")) {
            m_params.adaptationRate = orchestratorPattern.parameters.at("adaptationRate");
        }
    }

    void NotifyOrchestrator() {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Propagate evolution state to orchestrator
        IntentTask evolutionIntent{
            SystemDomain::Evolution,
            OctaveParams{
                .intensity = m_evolutionState.currentFitness,
                .coherence = m_params.stabilityThreshold,
                .variation = m_params.mutationRate
            },
            1.0f,  // High priority for evolution updates
            true   // Propagate to connected systems
        };
        
        orchestrator.PropagateIntent(evolutionIntent);
    }

    void InitializeEvolution(const ProceduralPatternData& source) {
        m_evolutionState.populationPool.clear();
        m_evolutionState.populationPool.push_back(source);
        
        // Generate initial population variations
        for (size_t i = 0; i < INITIAL_POPULATION_SIZE; ++i) {
            auto variant = CreateVariant(source, m_params.mutationRate);
            if (ValidatePattern(variant)) {
                m_evolutionState.populationPool.push_back(variant);
            }
        }
    }

    void EvolvePattern(const OctaveParams& rules, const HarmonyParams& constraints) {
        while (m_evolutionState.currentGeneration < m_params.generationLimit) {
            // Create new generation
            std::vector<ProceduralPatternData> newGeneration;
            
            // Selection
            auto parents = SelectParents(m_evolutionState.populationPool);
            
            // Crossover
            for (const auto& pair : parents) {
                if (Random::Float() < m_params.crossoverRate) {
                    auto offspring = Crossover(pair.first, pair.second);
                    if (ValidatePattern(offspring)) {
                        newGeneration.push_back(offspring);
                    }
                }
            }
            
            // Mutation
            for (auto& pattern : newGeneration) {
                if (Random::Float() < m_params.mutationRate) {
                    Mutate(pattern, rules);
                }
            }
            
            // Environmental Adaptation
            AdaptToEnvironment(newGeneration, constraints);
            
            // Evaluate Fitness
            EvaluatePopulation(newGeneration);
            
            // Update State
            UpdateEvolutionState(newGeneration);
            
            // Check for convergence
            if (HasConverged()) break;
            
            m_evolutionState.currentGeneration++;
        }
    }

    ProceduralPatternData Crossover(
        const ProceduralPatternData& parent1, 
        const ProceduralPatternData& parent2) 
    {
        ProceduralPatternData offspring;
        
        // Blend structural parameters
        offspring.structureParams = BlendStructureParams(
            parent1.structureParams,
            parent2.structureParams,
            m_params.crossoverRate
        );
        
        // Combine pattern-specific parameters
        for (const auto& [key, value] : parent1.parameters) {
            float blendFactor = Random::Float();
            offspring.parameters[key] = Lerp(
                value,
                parent2.parameters[key],
                blendFactor
            );
        }
        
        return offspring;
    }

    void Mutate(ProceduralPatternData& pattern, const OctaveParams& rules) {
        // Apply random variations based on octave parameters
        pattern.structureParams.regularity += 
            Random::Gaussian() * rules.lowEnd * m_params.mutationRate;
        
        pattern.structureParams.complexity += 
            Random::Gaussian() * rules.highEnd * m_params.mutationRate;
        
        pattern.structureParams.variation += 
            Random::Gaussian() * rules.intensity * m_params.mutationRate;
        
        // Mutate pattern-specific parameters
        for (auto& [key, value] : pattern.parameters) {
            value += Random::Gaussian() * rules.coherence * m_params.mutationRate;
            value = Clamp(value, 0.0f, 1.0f);
        }
    }

    void AdaptToEnvironment(
        std::vector<ProceduralPatternData>& population,
        const HarmonyParams& constraints) 
    {
        for (auto& pattern : population) {
            // Adjust pattern based on environmental constraints
            float adaptationFactor = m_params.adaptationRate * constraints.influence;
            
            // Adapt to system balance
            pattern.structureParams.regularity = 
                Lerp(pattern.structureParams.regularity, 
                     constraints.balance, 
                     adaptationFactor);
            
            // Apply domain-specific adaptations
            for (const auto& [domain, weight] : constraints.weights) {
                ApplyDomainAdaptation(pattern, domain, weight, adaptationFactor);
            }
        }
    }

    bool HasConverged() {
        // Check if we've reached optimal fitness
        if (m_evolutionState.currentFitness >= m_params.stabilityThreshold) {
            return true;
        }
        
        // Check if we're no longer improving significantly
        if (m_evolutionState.currentGeneration > MIN_GENERATIONS) {
            float improvement = m_evolutionState.currentFitness - 
                              m_evolutionState.bestFitness;
            return improvement < CONVERGENCE_THRESHOLD;
        }
        
        return false;
    }
};

} // namespace hd
