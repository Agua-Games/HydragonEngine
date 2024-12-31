#pragma once
#include "MemoryBenchmark.h"
#include <string>
#include <sstream>

namespace Hydragon {

class MemoryVisualization {
public:
    // Generate HTML report with interactive charts
    static std::string GenerateHTMLReport(const std::vector<MemoryBenchmark::BenchmarkResult>& results);
    
    // Generate console-based visualization
    static std::string GenerateConsoleReport(const MemoryBenchmark::BenchmarkResult& result);
    
    // Generate memory map visualization
    static std::string GenerateMemoryMap(const MemoryBenchmark::BenchmarkResult& result);
    
    // Export data for external tools
    static bool ExportToJSON(const std::vector<MemoryBenchmark::BenchmarkResult>& results, 
                           const std::string& filepath);

private:
    static std::string GenerateAccessPatternChart(const BenchmarkResult::AccessPatternMetrics& metrics);
    static std::string GenerateFragmentationVisual(const BenchmarkResult::FragmentationMetrics& metrics);
    static std::string GenerateHotspotHeatmap(const std::vector<BenchmarkResult::AccessPatternMetrics::HotSpot>& hotspots);
};

} // namespace Hydragon 