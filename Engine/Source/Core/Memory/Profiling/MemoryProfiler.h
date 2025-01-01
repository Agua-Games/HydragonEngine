/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory profiler for Hydragon
 */

#pragma once
#include "../Management/MemoryManager.h"
#include "MemoryBenchmark.h"
#include <chrono>

namespace Hydragon {
namespace Memory {

class MemoryProfiler {
public:
    void BeginCapture(const char* tag);
    void EndCapture();
    
    void EnableProfiling(bool enable);
    BenchmarkResult GetLastResult() const;
    
    // Integration with visualization
    void GenerateReport(const char* outputPath);
    
private:
    std::unique_ptr<MemoryBenchmark> m_Benchmark;
    // ... (profiling internals)
};

}} // namespace Hydragon::Memory 