/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DataTable.h
 * @brief DataTable is a class that handles data tables.
 * 
 * ARCHITECTURAL NOTES:
 * - DataTable is a singleton class that manages the data tables.
 * - It is responsible for creating, updating, and deleting data tables.
 * - It uses the Vulkan Memory Allocator (VMA) for GPU memory management.
 * - It uses the Vulkan Descriptor Set Layout for descriptor set management.
 * - It uses the Vulkan Descriptor Pool for descriptor pool management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"

namespace hd {

struct DataTableInfo : public NodeInfo {
    DataTableInfo() {
        NodeType = "Data/DataTable";
        inputs = {
            "Data",        // Data to be stored in the table
            "Columns",     // Column definitions
            "Rows",        // Row definitions
            "Filters",     // Filter definitions
            "Sorts"        // Sort definitions
        };
        outputs = {
            "Data",        // Data from the table
            "Metrics"      // Performance and quality metrics
        };
    }
};

class DataTable : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DataTable(const NodeInfo& info = NodeInfo())
        : Node(info) {}    // Call the base class constructor
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
    ~DataTable() = default;     // Default destructor
};

} // namespace hd
