/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file QualityAdaptation.h
 * @brief Header file for the QualityAdaptation class.
 * 
 * ARCHITECTURAL NOTES:
 * - QualityAdaptation is a class that represents a quality adaptation in Hydragon.
 * - It is used to adapt the quality of a game based on the performance of the game.
 * - It is a part of the Scalability module in Hydragon.
 */
#pragma once
#include "Core.h"
#include "Node.h"

namespace hd {

enum class Performance {
    Low,
    Medium,
    High,
    Maximum
};

struct QualityAdaptationInfo : public NodeInfo {
    QualityAdaptationInfo() {
        nodeType = "Scalability/QualityAdaptation";
        
        inputs = {
            "performance",     // Performance data
            "qualitySettings"  // Quality settings
        };
        
        outputs = {
            "adaptedQuality",  // Adapted quality settings
            "adaptationMetrics" // Adaptation performance metrics
        };
    }
};

class QualityAdaptation : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit QualityAdaptation(const QualityAdaptationInfo& info = QualityAdaptationInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    float adaptationInterval = 1.0f;
    std::map<Performance, float> qualityThresholds;
    bool dynamicLOD = false;
    float performanceTarget = 60.0f;

    // === Processing ===
    void setAdaptationInterval(float interval);
    void setQualityThresholds(const std::map<Performance, float>& thresholds);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~QualityAdaptation() = default;     // Default destructor
};

} // namespace hd
