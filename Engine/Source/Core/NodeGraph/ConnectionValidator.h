/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * HD_ConnectionValidator ensures node connections are valid
 * and provides meaningful feedback for invalid connections.
 */
#pragma once
#include <string>
#include "HD_Node.h"

namespace hd {

/**
 * Result of a connection validation check
 */
struct ConnectionValidationResult {
    bool isValid;
    std::string errorMessage;
    std::string warningMessage;
    float compatibilityScore; // 0.0 to 1.0, higher is better
    bool requiresConversion;
    std::string conversionMethod;
};

/**
 * Validates connections between nodes and provides feedback
 */
class HD_ConnectionValidator {
public:
    // Basic validation
    static ConnectionValidationResult ValidateConnection(
        HD_Node* sourceNode, const std::string& outputPort,
        HD_Node* targetNode, const std::string& inputPort);
    
    // Advanced validation
    static ConnectionValidationResult ValidateGraphIntegrity(const std::vector<HD_Node*>& nodes);
    static bool DetectCycles(const std::vector<HD_Node*>& nodes);
    static std::vector<HD_Node*> FindDisconnectedNodes(const std::vector<HD_Node*>& nodes);
    
    // Validation visualization
    static ImColor GetCompatibilityColor(float compatibilityScore);
    static std::string GetCompatibilityTooltip(const ConnectionValidationResult& result);
    
    // Suggestion system
    static std::vector<std::pair<HD_Node*, std::string>> SuggestCompatibleOutputs(
        HD_Node* targetNode, const std::string& inputPort);
    static std::vector<std::pair<HD_Node*, std::string>> SuggestCompatibleInputs(
        HD_Node* sourceNode, const std::string& outputPort);
};

} // namespace hd