/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * GraphAnalyzer provides tools to analyze node graphs for
 * potential issues, dead ends, and breaking points.
 */
#pragma once
#include <vector>
#include <string>
#include <map>
#include "Node.h"

namespace hd {

/**
 * Severity level for graph analysis issues
 */
enum class GraphIssueSeverity {
    Info,
    Warning,
    Error,
    Critical
};

/**
 * Represents an issue found during graph analysis
 */
struct GraphIssue {
    std::string description;
    GraphIssueSeverity severity;
    std::vector<Node*> relatedNodes;
    std::vector<std::pair<Node*, std::string>> relatedPorts;
    std::string suggestedFix;
};

/**
 * Analyzes node graphs for potential issues
 */
class GraphAnalyzer {
public:
    // Core analysis
    static std::vector<GraphIssue> analyzeGraph(const std::vector<Node*>& nodes);
    
    // Specific analyses
    static std::vector<GraphIssue> findDeadEnds(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> findUnusedOutputs(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> findMissingInputs(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> findPerformanceBottlenecks(const std::vector<Node*>& nodes);
    
    // Graph metrics
    static int calculateGraphComplexity(const std::vector<Node*>& nodes);
    static std::map<std::string, int> getNodeTypeDistribution(const std::vector<Node*>& nodes);
    static int getMaxGraphDepth(const std::vector<Node*>& nodes);
    
    // Visualization helpers
    static void highlightIssues(const std::vector<GraphIssue>& issues);
    static void generateGraphReport(const std::vector<Node*>& nodes, const std::string& outputPath);
};

} // namespace hd