/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory visualization for Hydragon
 */

#include "MemoryVisualization.h"
#include <algorithm>

namespace Hydragon {

std::string MemoryVisualization::GenerateConsoleReport(const MemoryBenchmark::BenchmarkResult& result) {
    std::stringstream ss;
    
    // Basic metrics
    ss << "=== Memory Benchmark Report ===\n";
    ss << "Test: " << result.name << "\n\n";
    
    // Performance metrics
    ss << "Performance:\n";
    ss << "├─ Average Time: " << result.averageTimeMs << "ms\n";
    ss << "├─ Throughput: " << result.throughputMBps << " MB/s\n";
    ss << "└─ Operations: " << result.operationCount << "\n\n";
    
    // Access pattern visualization
    if (!result.accessPatternMetrics.hotSpots.empty()) {
        ss << "Memory Access Heatmap:\n";
        
        // Simple ASCII heatmap
        for (const auto& hotspot : result.accessPatternMetrics.hotSpots) {
            double intensity = hotspot.accessFrequency / 
                result.accessPatternMetrics.hotSpots[0].accessFrequency;
            
            ss << "[";
            int bars = static_cast<int>(intensity * 20);
            ss << std::string(bars, '|');
            ss << std::string(20 - bars, ' ');
            ss << "] " << hotspot.accessCount << " accesses\n";
        }
        ss << "\n";
    }
    
    // Fragmentation visualization
    if (result.fragMetrics.fragmentationRatio > 0) {
        ss << "Memory Fragmentation:\n";
        int fragBlocks = static_cast<int>(result.fragMetrics.fragmentationRatio * 40);
        ss << "[";
        for (int i = 0; i < 40; ++i) {
            ss << (i < fragBlocks ? "▓" : "░");
        }
        ss << "] " << (result.fragMetrics.fragmentationRatio * 100) << "%\n\n";
    }
    
    return ss.str();
}

std::string MemoryVisualization::GenerateHTMLReport(
    const std::vector<MemoryBenchmark::BenchmarkResult>& results) {
    std::stringstream ss;
    
    // Basic HTML structure with embedded Chart.js
    ss << R"(
<!DOCTYPE html>
<html>
<head>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        .chart-container { width: 800px; margin: 20px auto; }
        .metric-card { 
            padding: 15px; 
            margin: 10px; 
            border: 1px solid #ccc; 
            border-radius: 5px; 
        }
    </style>
</head>
<body>
    )";
    
    // Add charts for each result
    for (const auto& result : results) {
        ss << GenerateAccessPatternChart(result.accessPatternMetrics);
        ss << GenerateFragmentationVisual(result.fragMetrics);
        ss << GenerateHotspotHeatmap(result.accessPatternMetrics.hotSpots);
    }
    
    ss << "</body></html>";
    return ss.str();
}

// Example usage:
void DisplayResults(const std::vector<MemoryBenchmark::BenchmarkResult>& results) {
    // Console output
    for (const auto& result : results) {
        std::cout << MemoryVisualization::GenerateConsoleReport(result);
    }
    
    // Generate HTML report
    std::string htmlReport = MemoryVisualization::GenerateHTMLReport(results);
    std::ofstream("memory_report.html") << htmlReport;
    
    // Export data for external tools
    MemoryVisualization::ExportToJSON(results, "memory_metrics.json");
} 