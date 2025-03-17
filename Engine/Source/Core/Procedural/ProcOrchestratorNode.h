/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcOrchestratorNode.h
 * @brief ProcOrchestratorNode is a singleton class that manages the orchestration of procedural patterns.
 * 
 * ARCHITECTURAL NOTES:
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include <memory>
#include <unordered_map>
#include <queue>

#include "ProceduralTypes.h"
#include "Node.h"

namespace hd {

struct ProcOrchestratorInfo : public NodeInfo {
    ProcOrchestratorInfo() {
        NodeType = "Procedural/Orchestrator";
        Name = "Procedural Orchestrator";
        
        // Global control inputs
        Inputs = {
            "GlobalIntent",          // Overall procedural behavior intent
            "HarmonyParams",         // System harmony parameters
            "TimeScale",             // Time scaling for evolution
            "Seed",                  // Global seed for consistency
            "SystemWeights"          // Weight map for different domains
        };
        
        // Orchestrated outputs
        Outputs = {
            "harmonizedPatterns",    // Collection of harmonized patterns
            "SystemStates",          // Current state of all systems
            "EvolutionMetrics",      // Pattern evolution metrics
            "PerformanceMetrics"     // Performance monitoring data
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

// Intent propagation task
struct IntentTask {
    SystemDomain domain;
    OctaveParams octaves;
    float priority;
    bool propagate;
};

class ProcOrchestratorNode : public Node {
public:
    static ProcOrchestratorNode& getInstance() {
        static ProcOrchestratorNode instance;
        return instance;
    }

    std::vector<std::string> getInputPorts() const override {
        return getNodeInfo().Inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return getNodeInfo().Outputs;
    }

    void processNodeGraph() override {
        // Process input intents and parameters
        auto globalIntent = getInputValue<OctaveParams>("GlobalIntent");
        auto harmonyParams = getInputValue<HarmonyParams>("HarmonyParams");
        float timeScale = getInputValue<float>("TimeScale");
        uint32_t seed = getInputValue<uint32_t>("Seed");
        
        // Update internal state
        setIntent(globalIntent);
        setHarmonyParams(harmonyParams);
        
        // Process pending intent tasks
        processIntentQueue(timeScale);
        
        // Balance and harmonize patterns
        balanceSystems();
        
        // Update outputs for connected nodes
        setOutputValue("harmonizedPatterns", getharmonizedPatterns());
        setOutputValue("SystemStates", getSystemStates());
        setOutputValue("EvolutionMetrics", computeEvolutionMetrics());
        setOutputValue("PerformanceMetrics", gatherPerformanceMetrics());
    }

    // Pattern Management (now supports node graph integration)
    std::string registerPattern(std::unique_ptr<IPattern> pattern) {
        std::string id = generatePatternId();
        patterns[id] = std::move(pattern);
        notifyPatternChanged(id);
        return id;
    }

    void unregisterPattern(const std::string& patternId) {
        patterns.erase(patternId);
        notifyPatternRemoved(patternId);
    }

    // Maestro Control
    void setIntent(const OctaveParams& intent) {
        globalIntent = intent;
    }

    void propagateIntent(const IntentTask& task) {
        intentQueue.push(task);
    }

    void updateHarmony(float deltaTime) {
        // Update harmony parameters
        // This is now handled in processNodeGraph
    }

    // System Orchestration
    void setHarmonyParams(const HarmonyParams& params) {
        harmonyParams = params;
    }

    void modulateSystem(SystemDomain domain, const OctaveParams& octaves) {
        // Modulate system parameters
        // This is now handled in processNodeGraph
    }

    void balanceSystems() {
        // Balance systems
        // This is now handled in processNodeGraph
    }

    // Procedural Pattern Interface
    ProceduralPatternData getProceduralPattern(const std::string& patternId) {
        auto it = patterns.find(patternId);
        if (it != patterns.end() && it->second->isProceduralPattern()) {
            return it->second-getProceduralData();
        }
        return {};
    }

    // Create procedural pattern helpers (now return pattern IDs for node connections)
    std::string createMaterialPattern(const ProceduralStructureParams& params) {
        auto pattern = std::make_unique<ProceduralPattern>(
            ProceduralPatternType::Material, 
            params
        );
        return registerPattern(std::move(pattern));
    }

    std::string createVolumeTexturePattern(const ProceduralStructureParams& params) {
        auto pattern = std::make_unique<ProceduralPattern>(
            ProceduralPatternType::VolumeTexture, 
            params
        );
        return registerPattern(std::move(pattern));
    }

    void drawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Procedural Orchestrator");
        
        // Draw input ports
        drawInputPort("GlobalIntent", "Intent");
        drawInputPort("HarmonyParams", "Harmony");
        drawInputPort("TimeScale", "Time");
        drawInputPort("Seed", "Seed");
        drawInputPort("SystemWeights", "Weights");
        
        // Draw output ports
        drawOutputPort("harmonizedPatterns", "Patterns");
        drawOutputPort("SystemStates", "States");
        drawOutputPort("EvolutionMetrics", "Evolution");
        drawOutputPort("PerformanceMetrics", "Performance");
        
        ImGui::EndGroup();
    }

private:
    ProcOrchestratorNode() 
        : Node(ProcOrchestratorInfo()) {}
    
    std::unordered_map<std::string, std::unique_ptr<IPattern>> patterns;
    std::queue<IntentTask> intentQueue;
    HarmonyParams harmonyParams;
    OctaveParams globalIntent;

    void processIntentQueue(float timeScale) {
        while (!intentQueue.empty()) {
            auto task = intentQueue.front();
            intentQueue.pop();
            
            if (task.propagate) {
                propagateIntent(task);
            }
        }
    }

    std::string generatePatternId() {
        return "pattern_" + std::to_string(patterns.size());
    }

    void notifyPatternChanged(const std::string& patternId) {
        // Notify connected nodes about pattern updates
        markOutputDirty("harmonizedPatterns");
    }

    void notifyPatternRemoved(const std::string& patternId) {
        // Notify connected nodes about pattern removal
        markOutputDirty("harmonizedPatterns");
    }

    // Prevent copying of singleton
    ProcOrchestratorNode(const ProcOrchestratorNode&) = delete;
    ProcOrchestratorNode& operator=(const ProcOrchestratorNode&) = delete;
};

} // namespace hd
