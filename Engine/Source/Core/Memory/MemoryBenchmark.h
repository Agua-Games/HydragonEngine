/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory benchmark for Hydragon
 */

#pragma once
#include "MemoryManager.h"
#include <chrono>
#include <vector>
#include <string>
#include <mutex>

namespace Hydragon {

class MemoryBenchmark {
public:
    struct BenchmarkResult {
        std::string name;
        double averageTimeMs;
        double minTimeMs;
        double maxTimeMs;
        size_t operationCount;
        size_t totalBytes;
        double throughputMBps;
        
        // Memory overhead
        size_t baselineMemory;
        size_t actualMemory;
        double overheadPercent;

        struct FragmentationMetrics {
            double fragmentationRatio;     // 0.0 to 1.0 (higher means more fragmented)
            size_t largestFreeBlock;
            size_t totalFreeBlocks;
            double averageBlockSize;
        };

        struct ThreadingMetrics {
            double contentionRatio;        // 0.0 to 1.0 (higher means more contention)
            uint32_t maxConcurrentThreads;
            double averageWaitTimeMs;
        };

        FragmentationMetrics fragMetrics;
        ThreadingMetrics threadMetrics;

        struct AccessPatternMetrics {
            uint32_t suspiciousPatterns;    // Count of potentially malicious patterns
            double burstAccessRatio;        // Ratio of burst vs normal access
            struct HotSpot {
                void* address;
                size_t accessCount;
                double accessFrequency;     // accesses per second
            };
            std::vector<HotSpot> hotSpots;  // Memory regions with high access frequency
        };

        AccessPatternMetrics accessPatternMetrics;
    };

    struct BenchmarkConfig {
        size_t iterationCount = 1000;
        size_t minAllocationSize = 8;
        size_t maxAllocationSize = 1024 * 1024;  // 1MB
        bool includeNetworkTests = false;
        bool includeSecurityTests = true;

        struct ThreadConfig {
            uint32_t threadCount = 4;
            bool measureContention = true;
            size_t operationsPerThread = 1000;
        };

        ThreadConfig threadConfig;
        bool measureFragmentation = true;

        struct PatternConfig {
            enum class WorkloadType {
                Graphics,
                Physics,
                Mixed
            };
            
            WorkloadType workloadType = WorkloadType::Mixed;
            bool trackAccessPatterns = true;
            uint32_t maxHotSpots = 10;      // Limit hotspot tracking for performance
            double burstThreshold = 1000.0;  // accesses/second to consider as burst
        };

        PatternConfig patternConfig;
    };

    static std::vector<BenchmarkResult> RunBenchmarks(const BenchmarkConfig& config = BenchmarkConfig());

private:
    static BenchmarkResult BenchmarkBasicAllocation(const BenchmarkConfig& config);
    static BenchmarkResult BenchmarkSecureAllocation(const BenchmarkConfig& config);
    static BenchmarkResult BenchmarkPoolAllocation(const BenchmarkConfig& config);
    static BenchmarkResult BenchmarkNetworkSecurity(const BenchmarkConfig& config);
    
    template<typename Func>
    static double MeasureTime(Func&& func);

    struct AccessPattern {
        void* address;
        size_t accessCount;
        std::chrono::high_resolution_clock::time_point timestamp;
        bool isBurst;
    };
    
    static std::vector<AccessPattern> s_AccessPatterns;
    static std::mutex s_PatternMutex;  // Thread safety for pattern collection
};

} // namespace Hydragon 