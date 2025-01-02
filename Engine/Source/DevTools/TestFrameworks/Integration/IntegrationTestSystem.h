/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Integration testing framework and execution system
 */

#pragma once
#include "TestTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::DevTools::Testing {

struct IntegrationTestConfig {
    bool enableEnvironmentIsolation = true;
    bool enableStateTracking = true;
    bool enableResourceMonitoring = true;
    bool enablePerformanceMetrics = true;
    uint32_t maxTestDuration = 300000;  // 5 minutes
    size_t maxMemoryUsage = 1024 * 1024 * 1024;  // 1GB
};

class IntegrationTestSystem {
public:
    static IntegrationTestSystem& Get();
    
    void Initialize(const IntegrationTestConfig& config = {});
    void Shutdown();
    
    void RegisterScenario(const std::string& name, const ScenarioCallback& callback);
    void RegisterEnvironment(const std::string& name, const EnvironmentSetup& setup);
    
    void RunAllScenarios();
    void RunScenario(const std::string& name);
    
    void SetupTestEnvironment(const std::string& name);
    void TeardownTestEnvironment();
    
    void CaptureState(const std::string& checkpoint);
    void RestoreState(const std::string& checkpoint);
    
    ScenarioResult GetLastResult() const;
    const IntegrationStats& GetStats() const { return m_Stats; }
    
    void GenerateReport(const std::string& path);

private:
    IntegrationTestSystem() = default;
    
    IntegrationTestConfig m_Config;
    IntegrationStats m_Stats;
    std::vector<TestScenario> m_Scenarios;
    std::unique_ptr<IScenarioRunner> m_Runner;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Testing 