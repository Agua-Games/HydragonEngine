/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnomalyDetector.h
 * @brief Header file for the AnomalyDetector class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnomalyDetector is a class that represents an anomaly detector in Hydragon.
 * - It is used to represent any anomaly detector in the game world, such as space anomaly detectors, etc.
 * - It supports interactive features, such as detecting, monitoring, and analyzing. Also supports two-way messaging with other anomaly detectors, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include "Engine.h"
#include "Node.h"
#include "Device.h"
#include "HazardZone.h"
#include "ScannerDevice.h"

namespace hd {

struct AnomalyDetectorInfo : public ScannerDeviceInfo {
    AnomalyDetectorInfo() {
        nodeType = "Gameplay/AnomalyDetector";
        
        inputs = {
            "anomalyDetectorType",  // Type of anomaly detector (space anomaly detector, etc.)
            "anomalyDetectorData",  // Anomaly detector data
            "environment",          // Environment data
            "characterData",        // Character data
            "anomalyDetectorState"  // Anomaly detector state
        };
        
        outputs = {
            "anomalyDetectorStatus",  // Anomaly detector status
            "anomalyDetectorMetrics"  // Anomaly detector performance metrics
        };
    }
};

class AnomalyDetector : public ScannerDevice {
public:
    // === Allocation, Initialization, Loading ===
    explicit AnomalyDetector(const AnomalyDetectorInfo& info = AnomalyDetectorInfo())
        : ScannerDevice(info) {}    // Initialize scanner device info struct with default values
    initialize() override {}
    load() override {}

    // Set default values
    AnomalyDetectorInfo anomalyDetectorInfo = {};  // Initialize anomaly detector info struct with default values
    std::vector<std::string> signatures;

    // === Processing ===
    void processAnomalyDetector();
    void processNode() override {
        processAnomalyDetector(); 
    }
    void scan();
    void addSignature(const std::string& signature);
    void removeSignature(const std::string& signature);
    void detect();
    void monitor();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AnomalyDetector() = default;     // Default destructor
};

} // namespace hd
