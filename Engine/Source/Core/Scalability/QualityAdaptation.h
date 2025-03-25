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

struct QualityAdaptationInfo : public NodeInfo {
    QualityAdaptationInfo() {
        NodeType = "Scalability/QualityAdaptation";
        
        inputs = {
            "Performance",     // Performance data
            "QualitySettings"  // Quality settings
        };
        
        outputs = {
            "AdaptedQuality",  // Adapted quality settings
            "AdaptationMetrics" // Adaptation performance metrics
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~QualityAdaptation() = default;     // Default destructor
};

} // namespace hd
