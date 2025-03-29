/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DataCurve.h
 * @brief Header file for the DataCurve class.
 * 
 * ARCHITECTURAL NOTES:
 * - DataCurve is a class that represents a data curve in Hydragon.
 * - It is used to represent and process data curves.
 * - It uses the Vulkan API for data curve processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DataCurveInfo : public NodeInfo {
    DataCurveInfo() {
        NodeType = "Data/DataCurve";
        inputs = {
            "Data",        // Data to be stored in the curve
            "CurveType",   // Type of curve
            "CurveParams"  // Curve parameters
        };
        outputs = {
            "Data",        // Data from the curve
            "Metrics"      // Performance and quality metrics
        };
    }
};

class DataCurve : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit DataCurve(const DataCurveInfo& info = DataCurveInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void processCurve();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DataCurve() = default;     // Default destructor
};

} // namespace hd
