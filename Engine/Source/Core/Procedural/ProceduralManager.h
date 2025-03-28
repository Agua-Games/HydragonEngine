/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralManager.h
 * @brief ProceduralManager represents a singleton class that manages the orchestration of procedural patterns.
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

struct ProceduralManagerInfo : public NodeInfo {
    ProceduralManagerInfo() {
        nodeType = "Procedural/ProceduralManager";
        name = "ProceduralManager";
        
        // Global control inputs
        inputs = {
            "globalIntent",          // Overall procedural behavior intent
            "harmonyParams",         // System harmony parameters
            "timeScale",             // Time scaling for evolution
            "seed",                  // Global seed for consistency
            "systemWeights"          // Weight map for different domains
        };
        
        // Orchestrated outputs
        outputs = {
            "harmonizedPatterns",    // Collection of harmonized patterns
            "systemStates",          // Current state of all systems
            "evolutionMetrics",      // Pattern evolution metrics
            "performanceMetrics",    // Performance monitoring data
            "systemMetrics"          // System metrics
        };

        isSerializable = true;
        isEditableInEditor = true;
        isProcedural = true;
    }
};

// Intent propagation task
struct IntentTask {
    SystemDomain domain;
    OctaveParams octaves;
    float priority;
    bool propagate;
};

/**
 * @class ProceduralManager
 * @brief Represents a procedural manager node in the engine's node graph.
 */
class ProceduralManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    ProceduralManager() 
        : Node(ProceduralManagerInfo()) {}
    initialize() override {}
    load() override {}

    // Singleton instance
    static ProceduralManager& getInstance() {
        static ProceduralManager instance;
        return instance;
    }

    // === Port Management ===
    std::vector<std::string> getInputPorts() const override {
        return getNodeInfo().inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return getNodeInfo().outputs;
    }

    // === Processing ===
    void processNode() override {
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
        setOutputValue("SystemMetrics", gatherSystemMetrics());
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
        // This is now handled in 
    }

    // System Orchestration
    void setHarmonyParams(const HarmonyParams& params) {
        harmonyParams = params;
    }

    void modulateSystem(SystemDomain domain, const OctaveParams& octaves) {
        // Modulate system parameters
        // This is now handled in 
    }

    void balanceSystems() {
        // Balance systems
        // This is now handled in 
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

    /**
     * @brief Draw this node in the Node Graph Editor.
     * Nodes will be automatically drawn based on their data types, structure, etc. But for custom visualization logic, override this method in derived classes.
     */
    void drawInNodeGraph() override {
    }

private:
    // === Allocation, Initialization, Loading ===  
    std::unordered_map<std::string, std::unique_ptr<IPattern>> patterns;
    std::queue<IntentTask> intentQueue;
    HarmonyParams harmonyParams;
    OctaveParams globalIntent;

    // === Processing ===
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

    // === Cleanup ===
    // Prevent copying of singleton
    ProceduralManager(const ProceduralManager&) = delete;
    ProceduralManager& operator=(const ProceduralManager&) = delete;
};

} // namespace hd
