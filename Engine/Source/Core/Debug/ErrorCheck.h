/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ErrorCheck.h
 * @brief Header file for the ErrorCheck class.
 * 
 * ARCHITECTURAL NOTES:
 * - ErrorCheck is a class that represents an error check in Hydragon.
 * - It is used to represent and process error checks.
 * - It uses the Vulkan API for error check processing.
 * 
 * @todo Declare-define all default member variables.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include <string>

namespace hd {

struct ErrorType {
    enum class Type {
        None,
        Warning,
        Error,
        Critical
    };
};

struct ErrorData {
    std::string message;
    std::string stackTrace;
    std::string sourceFile;
    int lineNumber;
};

struct ErrorCheckInfo : public NodeInfo {
    ErrorCheckInfo() {
        NodeType = "Debug/ErrorCheck";
        
        inputs = {
            "ErrorType",  // Type of error to check for
            "ErrorData"   // Data associated with the error
        };
        
        outputs = {
            "ErrorStatus" // Status of the error check
        };
    }
};

class ErrorCheck : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ErrorCheck(const ErrorCheckInfo& info = ErrorCheckInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ErrorCheck() = default;     // Default destructor
};

} // namespace hd
