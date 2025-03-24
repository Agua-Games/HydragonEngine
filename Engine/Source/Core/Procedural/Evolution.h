/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Evolution.h
 * @brief Evolution represents an evolution node in the engine's node graph.
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

#include "ProceduralTypes.h"
#include "ProcOrchestrator.h"
#include "Node.h"

namespace hd {

struct EvolutionInfo : public NodeInfo {
    EvolutionInfo() {
        NodeType = "Procedural/Evolution";
        
        inputs = {
            "SourcePattern",
            "TimeScale",
            "EvolutionRules",
            "Constraints"
        };
        
        outputs = {
            "EvolvedPattern",
            "EvolutionState",
            "TransitionMetrics"
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Evolution : public Node {
public:
    explicit Evolution(const EvolutionInfo& info = EvolutionInfo())
        : Node(info) {
        // Register with orchestrator on creation
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        m_evolutionPatternId = orchestrator.registerPattern(
            std::make_unique<ProceduralPattern>(
                ProceduralPatternType::Evolution,
                createInitialEvolutionParams()
            )
        );
    }

    ~Evolution() {
        // Cleanup registration
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        orchestrator.unregisterPattern(m_evolutionPatternId);
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Get inputs
        auto sourcePattern = getInputValue<ProceduralPatternData>("SourcePattern");
        auto evolutionRules = getInputValue<OctaveParams>("EvolutionRules");
        auto constraints = getInputValue<HarmonyParams>("Constraints");
        
        // Initialize evolution if needed
        if (!m_evolutionState.currentGeneration) {
            InitializeEvolution(sourcePattern);
        }

        // Get orchestrator's fine-tuning parameters
        auto orchestratorPattern = orchestrator.getProceduralPattern(m_evolutionPatternId);
        updateEvolutionParams(orchestratorPattern);

        // evolve pattern with orchestrator guidance
        evolvePattern(evolutionRules, constraints);
        
        // Update orchestrator about evolution progress
        eotifyOrchestrator();
        
        // Set outputs
        setOutputValue("EvolvedPattern", m_evolutionState.bestPattern);
        setOutputValue("EvolutionState", getEvolutionMetrics());
        setOutputValue("TransitionMetrics", calculateTransitionMetrics());
    }

private:
    EvolutionState m_evolutionState;
    EvolutionParameters m_params;
    std::string m_evolutionPatternId;

    void updateEvolutionParams(const ProceduralPatternData& orchestratorPattern) {
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

    void notifyOrchestrator() {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
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
        
        orchestrator.propagateIntent(evolutionIntent);
    }

    void initializeEvolution(const ProceduralPatternData& source) {
        m_evolutionState.populationPool.clear();
        m_evolutionState.populationPool.push_back(source);
        
        // Generate initial population variations
        for (size_t i = 0; i < INITIAL_POPULATION_SIZE; ++i) {
            auto variant = createVariant(source, m_params.mutationRate);
            if (validatePattern(variant)) {
                m_evolutionState.populationPool.push_back(variant);
            }
        }
    }

    void evolvePattern(const OctaveParams& rules, const HarmonyParams& constraints) {
        while (m_evolutionState.currentGeneration < m_params.generationLimit) {
            // Create new generation
            std::vector<ProceduralPatternData> newGeneration;
            
            // Selection
            auto parents = selectParents(m_evolutionState.populationPool);
            
            // Crossover
            for (const auto& pair : parents) {
                if (Random::Float() < m_params.crossoverRate) {
                    auto offspring = crossover(pair.first, pair.second);
                    if (validatePattern(offspring)) {
                        newGeneration.push_back(offspring);
                    }
                }
            }
            
            // Mutation
            for (auto& pattern : newGeneration) {
                if (Random::Float() < m_params.mutationRate) {
                    mutate(pattern, rules);
                }
            }
            
            // Environmental Adaptation
            adaptToEnvironment(newGeneration, constraints);
            
            // Evaluate Fitness
            evaluatePopulation(newGeneration);
            
            // Update State
            updateEvolutionState(newGeneration);
            
            // Check for convergence
            if (hasConverged()) break;
            
            m_evolutionState.currentGeneration++;
        }
    }

    ProceduralPatternData crossover(
        const ProceduralPatternData& parent1, 
        const ProceduralPatternData& parent2) 
    {
        ProceduralPatternData offspring;
        
        // Blend structural parameters
        offspring.structureParams = blendStructureParams(
            parent1.structureParams,
            parent2.structureParams,
            m_params.crossoverRate
        );
        
        // Combine pattern-specific parameters
        for (const auto& [key, value] : parent1.parameters) {
            float blendFactor = Random::Float();
            offspring.parameters[key] = lerp(
                value,
                parent2.parameters[key],
                blendFactor
            );
        }
        
        return offspring;
    }

    void mutate(ProceduralPatternData& pattern, const OctaveParams& rules) {
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
            value = clamp(value, 0.0f, 1.0f);
        }
    }

    void adaptToEnvironment(
        std::vector<ProceduralPatternData>& population,
        const HarmonyParams& constraints) 
    {
        for (auto& pattern : population) {
            // Adjust pattern based on environmental constraints
            float adaptationFactor = m_params.adaptationRate * constraints.influence;
            
            // adapt to system balance
            pattern.structureParams.regularity = 
                lerp(pattern.structureParams.regularity, 
                     constraints.balance, 
                     adaptationFactor);
            
            // Apply domain-specific adaptations
            for (const auto& [domain, weight] : constraints.weights) {
                applyDomainAdaptation(pattern, domain, weight, adaptationFactor);
            }
        }
    }

    bool hasConverged() {
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
