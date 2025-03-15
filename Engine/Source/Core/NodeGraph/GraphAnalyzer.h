/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * HD_GraphAnalyzer provides tools to analyze node graphs for
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
class HD_GraphAnalyzer {
public:
    // Core analysis
    static std::vector<GraphIssue> AnalyzeGraph(const std::vector<Node*>& nodes);
    
    // Specific analyses
    static std::vector<GraphIssue> FindDeadEnds(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> FindUnusedOutputs(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> FindMissingInputs(const std::vector<Node*>& nodes);
    static std::vector<GraphIssue> FindPerformanceBottlenecks(const std::vector<Node*>& nodes);
    
    // Graph metrics
    static int CalculateGraphComplexity(const std::vector<Node*>& nodes);
    static std::map<std::string, int> GetNodeTypeDistribution(const std::vector<Node*>& nodes);
    static int GetMaxGraphDepth(const std::vector<Node*>& nodes);
    
    // Visualization helpers
    static void HighlightIssues(const std::vector<GraphIssue>& issues);
    static void GenerateGraphReport(const std::vector<Node*>& nodes, const std::string& outputPath);
};

} // namespace hd