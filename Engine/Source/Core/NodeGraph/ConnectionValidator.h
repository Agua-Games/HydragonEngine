/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief ConnectionValidator ensures node connections are valid
 * and provides meaningful feedback for invalid connections.
 */
#pragma once
#include <map>
#include <string>
#include <vector>
#include "Node.h"
#include "NodeTypeSystem.h"

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
class ConnectionValidator {
public:
    // Basic validation
    static ConnectionValidationResult validateConnection(
        Node* sourceNode, const std::string& outputPort,
        Node* targetNode, const std::string& inputPort);
    
    // Advanced validation
    static ConnectionValidationResult validateGraphIntegrity(const std::vector<Node*>& nodes);
    static bool detectCycles(const std::vector<Node*>& nodes);
    static std::vector<Node*> findDisconnectedNodes(const std::vector<Node*>& nodes);
    
    // Validation visualization
    static ImColor getCompatibilityColor(float compatibilityScore);
    static std::string getCompatibilityTooltip(const ConnectionValidationResult& result);
    
    // Suggestion system
    static std::vector<std::pair<Node*, std::string>> suggestCompatibleOutputs(
        Node* targetNode, const std::string& inputPort);
    static std::vector<std::pair<Node*, std::string>> suggestCompatibleInputs(
        Node* sourceNode, const std::string& outputPort);
};

} // namespace hd