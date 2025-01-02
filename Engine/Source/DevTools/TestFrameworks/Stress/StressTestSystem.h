/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Stress testing and load simulation system
 */

#pragma once
#include "TestTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::DevTools::Testing {

struct StressTestConfig {
    bool enableLoadGeneration = true;
    bool enableResourceMonitoring = true;
    bool enableCrashRecovery = true;
    bool enableMetricsCollection = true;
    uint32_t maxConcurrentUsers = 1000;
    uint32_t testDuration = 3600;  // 1 hour
    float rampUpTime = 300.0f;  // 5 minutes
};

class StressTestSystem {
public:
    static StressTestSystem& Get();
    
    void Initialize(const StressTestConfig& config = {});
    void Shutdown();
    
    void RegisterScenario(const std::string& name, const LoadScenario& scenario);
    void RegisterMetric(const std::string& name, const MetricCallback& callback);
    
    void RunLoadTest(const std::string& scenario);
    void StopLoadTest();
    
    void SetLoadProfile(const LoadProfile& profile);
    void SetResourceLimits(const ResourceLimits& limits);
    
    LoadTestResult GetCurrentResults() const;
    const StressTestStats& GetStats() const { return m_Stats; }
    
    void GenerateReport(const std::string& path);
    void ExportMetrics(const std::string& path);

private:
    StressTestSystem() = default;
    
    StressTestConfig m_Config;
    StressTestStats m_Stats;
    std::vector<LoadScenario> m_Scenarios;
    std::unique_ptr<ILoadGenerator> m_Generator;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Testing 