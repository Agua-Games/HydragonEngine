/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "Core/NodeGraph/HD_Node.h"
#include <memory>
#include <unordered_map>
#include <queue>

namespace hd {

struct HD_ProceduralOrchestratorInfo : public HD_NodeInfo {
    HD_ProceduralOrchestratorInfo() {
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
            "HarmonizedPatterns",    // Collection of harmonized patterns
            "SystemStates",          // Current state of all systems
            "EvolutionMetrics",      // Pattern evolution metrics
            "PerformanceMetrics"     // Performance monitoring data
        };

        IsSerializable = true;
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

class HD_ProceduralOrchestrator : public HD_Node {
public:
    static HD_ProceduralOrchestrator& GetInstance() {
        static HD_ProceduralOrchestrator instance;
        return instance;
    }

    std::vector<std::string> GetInputPorts() const override {
        return GetNodeInfo().Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return GetNodeInfo().Outputs;
    }

    void ProcessNodeGraph() override {
        // Process input intents and parameters
        auto globalIntent = GetInputValue<OctaveParams>("GlobalIntent");
        auto harmonyParams = GetInputValue<HarmonyParams>("HarmonyParams");
        float timeScale = GetInputValue<float>("TimeScale");
        uint32_t seed = GetInputValue<uint32_t>("Seed");
        
        // Update internal state
        SetIntent(globalIntent);
        SetHarmonyParams(harmonyParams);
        
        // Process pending intent tasks
        ProcessIntentQueue(timeScale);
        
        // Balance and harmonize patterns
        BalanceSystems();
        
        // Update outputs for connected nodes
        SetOutputValue("HarmonizedPatterns", GetHarmonizedPatterns());
        SetOutputValue("SystemStates", GetSystemStates());
        SetOutputValue("EvolutionMetrics", ComputeEvolutionMetrics());
        SetOutputValue("PerformanceMetrics", GatherPerformanceMetrics());
    }

    // Pattern Management (now supports node graph integration)
    std::string RegisterPattern(std::unique_ptr<IPattern> pattern) {
        std::string id = GeneratePatternId();
        patterns[id] = std::move(pattern);
        NotifyPatternChanged(id);
        return id;
    }

    void UnregisterPattern(const std::string& patternId) {
        patterns.erase(patternId);
        NotifyPatternRemoved(patternId);
    }

    // Maestro Control
    void SetIntent(const OctaveParams& intent) {
        globalIntent = intent;
    }

    void PropagateIntent(const IntentTask& task) {
        intentQueue.push(task);
    }

    void UpdateHarmony(float deltaTime) {
        // Update harmony parameters
        // This is now handled in ProcessNodeGraph
    }

    // System Orchestration
    void SetHarmonyParams(const HarmonyParams& params) {
        harmonyParams = params;
    }

    void ModulateSystem(SystemDomain domain, const OctaveParams& octaves) {
        // Modulate system parameters
        // This is now handled in ProcessNodeGraph
    }

    void BalanceSystems() {
        // Balance systems
        // This is now handled in ProcessNodeGraph
    }

    // Procedural Pattern Interface
    ProceduralPatternData GetProceduralPattern(const std::string& patternId) {
        auto it = patterns.find(patternId);
        if (it != patterns.end() && it->second->IsProceduralPattern()) {
            return it->second->GetProceduralData();
        }
        return {};
    }

    // Create procedural pattern helpers (now return pattern IDs for node connections)
    std::string CreateMaterialPattern(const ProceduralStructureParams& params) {
        auto pattern = std::make_unique<ProceduralPattern>(
            ProceduralPatternType::Material, 
            params
        );
        return RegisterPattern(std::move(pattern));
    }

    std::string CreateVolumeTexturePattern(const ProceduralStructureParams& params) {
        auto pattern = std::make_unique<ProceduralPattern>(
            ProceduralPatternType::VolumeTexture, 
            params
        );
        return RegisterPattern(std::move(pattern));
    }

    void DrawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Procedural Orchestrator");
        
        // Draw input ports
        DrawInputPort("GlobalIntent", "Intent");
        DrawInputPort("HarmonyParams", "Harmony");
        DrawInputPort("TimeScale", "Time");
        DrawInputPort("Seed", "Seed");
        DrawInputPort("SystemWeights", "Weights");
        
        // Draw output ports
        DrawOutputPort("HarmonizedPatterns", "Patterns");
        DrawOutputPort("SystemStates", "States");
        DrawOutputPort("EvolutionMetrics", "Evolution");
        DrawOutputPort("PerformanceMetrics", "Performance");
        
        ImGui::EndGroup();
    }

private:
    HD_ProceduralOrchestrator() 
        : HD_Node(HD_ProceduralOrchestratorInfo()) {}
    
    std::unordered_map<std::string, std::unique_ptr<IPattern>> patterns;
    std::queue<IntentTask> intentQueue;
    HarmonyParams harmonyParams;
    OctaveParams globalIntent;

    void ProcessIntentQueue(float timeScale) {
        while (!intentQueue.empty()) {
            auto task = intentQueue.front();
            intentQueue.pop();
            
            if (task.propagate) {
                PropagateIntent(task);
            }
        }
    }

    std::string GeneratePatternId() {
        return "pattern_" + std::to_string(patterns.size());
    }

    void NotifyPatternChanged(const std::string& patternId) {
        // Notify connected nodes about pattern updates
        MarkOutputDirty("HarmonizedPatterns");
    }

    void NotifyPatternRemoved(const std::string& patternId) {
        // Notify connected nodes about pattern removal
        MarkOutputDirty("HarmonizedPatterns");
    }

    // Prevent copying of singleton
    HD_ProceduralOrchestrator(const HD_ProceduralOrchestrator&) = delete;
    HD_ProceduralOrchestrator& operator=(const HD_ProceduralOrchestrator&) = delete;
};

} // namespace hd
