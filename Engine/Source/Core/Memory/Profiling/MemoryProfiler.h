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