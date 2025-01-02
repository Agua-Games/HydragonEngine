/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Performance testing and benchmarking system
 */

#pragma once
#include "TestTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::DevTools::Testing {

struct PerformanceTestConfig {
    bool enableDetailedProfiling = true;
    bool enableBaselineComparison = true;
    bool enableThresholdChecks = true;
    bool enableStressTests = true;
    uint32_t iterationCount = 1000;
    uint32_t warmupIterations = 100;
    float thresholdTolerance = 0.1f;  // 10%
};

class PerformanceTestSystem {
public:
    static PerformanceTestSystem& Get();
    
    void Initialize(const PerformanceTestConfig& config = {});
    void Shutdown();
    
    void RegisterBenchmark(const std::string& name, const BenchmarkCallback& callback);
    void RegisterBaseline(const std::string& name, const BaselineMetrics& baseline);
    
    void RunAllBenchmarks();
    void RunBenchmark(const std::string& name);
    
    void SetThreshold(const std::string& metric, float threshold);
    void ClearThresholds();
    
    BenchmarkResult GetLastResult() const;
    const PerformanceStats& GetStats() const { return m_Stats; }
    
    void GenerateReport(const std::string& path);
    void ExportMetrics(const std::string& path);

private:
    PerformanceTestSystem() = default;
    
    PerformanceTestConfig m_Config;
    PerformanceStats m_Stats;
    std::vector<Benchmark> m_Benchmarks;
    std::unique_ptr<IBenchmarkRunner> m_Runner;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Testing 