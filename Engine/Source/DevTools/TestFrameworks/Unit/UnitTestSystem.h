/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Unit testing framework and execution system
 */

#pragma once
#include "TestTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::DevTools::Testing {

struct UnitTestConfig {
    bool enableParallelExecution = true;
    bool enableCoverage = true;
    bool enableBenchmarking = true;
    bool enableMocking = true;
    uint32_t maxParallelTests = 8;
    uint32_t testTimeout = 5000;  // ms
    std::string outputDirectory = "TestResults";
};

class UnitTestSystem {
public:
    static UnitTestSystem& Get();
    
    void Initialize(const UnitTestConfig& config = {});
    void Shutdown();
    
    void RegisterTest(const std::string& suite, const std::string& name, 
                     const TestCallback& callback);
    void RegisterFixture(const std::string& suite, const TestFixture& fixture);
    
    void RunAll();
    void RunSuite(const std::string& suite);
    void RunTest(const std::string& suite, const std::string& name);
    
    void SetupMock(const std::string& name, const MockCallback& callback);
    void ClearMocks();
    
    TestResult GetLastResult() const;
    const TestStats& GetStats() const { return m_Stats; }
    
    void GenerateReport(const std::string& path);

private:
    UnitTestSystem() = default;
    
    UnitTestConfig m_Config;
    TestStats m_Stats;
    std::vector<TestSuite> m_Suites;
    std::unique_ptr<ITestRunner> m_Runner;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Testing 