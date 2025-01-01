/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory benchmark for Hydragon
 */

#include "MemoryBenchmark.h"
#include <algorithm>
#include <random>

namespace Hydragon {

std::vector<MemoryBenchmark::BenchmarkResult> MemoryBenchmark::RunBenchmarks(const BenchmarkConfig& config) {
    std::vector<BenchmarkResult> results;
    
    // Baseline measurement
    auto baseline = BenchmarkBasicAllocation(config);
    results.push_back(baseline);
    
    if (config.includeSecurityTests) {
        // Test with security features
        auto& mm = MemoryManager::Get();
        MemoryManager::SecurityConfig secConfig;
        secConfig.enableCanaries = true;
        secConfig.randomizePoolSizes = true;
        mm.SetSecurityConfig(secConfig);
        
        auto secure = BenchmarkSecureAllocation(config);
        results.push_back(secure);
    }
    
    // Pool allocation benchmark
    auto pool = BenchmarkPoolAllocation(config);
    results.push_back(pool);
    
    if (config.includeNetworkTests) {
        auto network = BenchmarkNetworkSecurity(config);
        results.push_back(network);
    }
    
    return results;
}

BenchmarkResult MemoryBenchmark::BenchmarkBasicAllocation(const BenchmarkConfig& config) {
    BenchmarkResult result;
    result.name = "Basic Allocation";
    
    std::vector<void*> allocations;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> size_dist(config.minAllocationSize, config.maxAllocationSize);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < config.iterationCount; ++i) {
        size_t size = size_dist(gen);
        void* ptr = MemoryManager::Get().Allocate(size);
        allocations.push_back(ptr);
        result.totalBytes += size;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    // Cleanup
    for (void* ptr : allocations) {
        MemoryManager::Get().Deallocate(ptr);
    }
    
    result.averageTimeMs = duration.count() / config.iterationCount;
    result.operationCount = config.iterationCount;
    result.throughputMBps = (result.totalBytes / 1024.0 / 1024.0) / (duration.count() / 1000.0);
    
    return result;
}

BenchmarkResult MemoryBenchmark::BenchmarkSecureAllocation(const BenchmarkConfig& config) {
    BenchmarkResult result;
    result.name = "Secure Allocation";
    
    std::vector<std::pair<void*, size_t>> allocations;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> size_dist(config.minAllocationSize, config.maxAllocationSize);
    
    auto& mm = MemoryManager::Get();
    
    // Measure baseline memory
    result.baselineMemory = mm.GetCurrentUsage();
    
    auto start = std::chrono::high_resolution_clock::now();
    double minTime = std::numeric_limits<double>::max();
    double maxTime = 0;
    
    for (size_t i = 0; i < config.iterationCount; ++i) {
        size_t size = size_dist(gen);
        
        auto alloc_start = std::chrono::high_resolution_clock::now();
        void* ptr = mm.SecureAllocate(size, alignof(std::max_align_t));
        auto alloc_end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> alloc_time = alloc_end - alloc_start;
        minTime = std::min(minTime, alloc_time.count());
        maxTime = std::max(maxTime, alloc_time.count());
        
        allocations.emplace_back(ptr, size);
        result.totalBytes += size;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    // Measure actual memory with security features
    result.actualMemory = mm.GetCurrentUsage();
    result.overheadPercent = ((double)result.actualMemory / result.baselineMemory - 1.0) * 100.0;
    
    // Cleanup
    for (const auto& alloc : allocations) {
        mm.Deallocate(alloc.first);
    }
    
    result.averageTimeMs = duration.count() / config.iterationCount;
    result.minTimeMs = minTime;
    result.maxTimeMs = maxTime;
    result.operationCount = config.iterationCount;
    result.throughputMBps = (result.totalBytes / 1024.0 / 1024.0) / (duration.count() / 1000.0);
    
    return result;
}

BenchmarkResult MemoryBenchmark::BenchmarkFragmentation(const BenchmarkConfig& config) {
    BenchmarkResult result;
    result.name = "Fragmentation Test";
    
    auto& mm = MemoryManager::Get();
    std::vector<void*> allocations;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Phase 1: Create fragmentation pattern
    std::uniform_int_distribution<size_t> size_dist(32, 4096);  // Realistic sizes
    std::uniform_int_distribution<int> free_dist(0, 1);  // 50% chance to free
    
    size_t totalAllocated = 0;
    size_t totalFragments = 0;
    
    for (size_t i = 0; i < config.iterationCount; ++i) {
        size_t size = size_dist(gen);
        void* ptr = mm.Allocate(size);
        
        if (ptr) {
            totalAllocated += size;
            if (free_dist(gen) == 0) {  // 50% chance to keep allocation
                allocations.push_back(ptr);
            } else {
                mm.Deallocate(ptr);
                totalFragments++;
            }
        }
    }
    
    // Measure fragmentation
    result.fragMetrics.totalFreeBlocks = totalFragments;
    result.fragMetrics.averageBlockSize = totalAllocated / 
        (allocations.size() + totalFragments);
    
    // Simple fragmentation ratio based on free blocks vs total blocks
    result.fragMetrics.fragmentationRatio = static_cast<double>(totalFragments) /
        (allocations.size() + totalFragments);
    
    // Cleanup
    for (void* ptr : allocations) {
        mm.Deallocate(ptr);
    }
    
    return result;
}

BenchmarkResult MemoryBenchmark::BenchmarkThreadSafety(const BenchmarkConfig& config) {
    BenchmarkResult result;
    result.name = "Thread Safety Test";
    
    std::vector<std::thread> threads;
    std::atomic<size_t> totalOps{0};
    std::atomic<size_t> contentionCount{0};
    
    auto threadFunc = [&](size_t threadId) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> size_dist(
            config.minAllocationSize, 
            config.maxAllocationSize
        );
        
        for (size_t i = 0; i < config.threadConfig.operationsPerThread; ++i) {
            size_t size = size_dist(gen);
            
            auto start = std::chrono::high_resolution_clock::now();
            void* ptr = MemoryManager::Get().Allocate(size);
            auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> duration = end - start;
            if (duration.count() > 0.1) { // Basic contention detection
                contentionCount++;
            }
            
            if (ptr) {
                MemoryManager::Get().Deallocate(ptr);
                totalOps++;
            }
        }
    };
    
    // Launch threads
    auto start = std::chrono::high_resolution_clock::now();
    
    for (uint32_t i = 0; i < config.threadConfig.threadCount; ++i) {
        threads.emplace_back(threadFunc, i);
    }
    
    // Wait for completion
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    // Calculate metrics
    result.threadMetrics.maxConcurrentThreads = config.threadConfig.threadCount;
    result.threadMetrics.contentionRatio = static_cast<double>(contentionCount) / totalOps;
    result.threadMetrics.averageWaitTimeMs = duration.count() / totalOps;
    
    return result;
}

BenchmarkResult MemoryBenchmark::BenchmarkGraphicsPattern(const BenchmarkConfig& config) {
    BenchmarkResult result;
    result.name = "Graphics Memory Pattern";
    
    struct GraphicsAlloc {
        size_t size;
        bool isTexture;
        bool isVertexData;
    };
    
    std::vector<GraphicsAlloc> typicalAllocations = {
        {4 * 1024 * 1024, true, false},    // 4MB texture
        {64 * 1024, false, true},          // 64KB vertex buffer
        {1024 * 1024, false, false},       // 1MB uniform buffer
        {2 * 1024 * 1024, true, false}     // 2MB render target
    };
    
    auto& mm = MemoryManager::Get();
    std::vector<void*> allocations;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simulate frame-based allocation pattern
    for (size_t frame = 0; frame < 60; ++frame) {  // Simulate 60 frames
        for (const auto& alloc : typicalAllocations) {
            void* ptr = mm.Allocate(alloc.size);
            if (ptr) {
                allocations.push_back(ptr);
                result.totalBytes += alloc.size;
                
                if (alloc.isTexture || alloc.isVertexData) {
                    // Simulate texture/vertex data access pattern
                    SimulateMemoryAccess(ptr, alloc.size);
                }
            }
        }
        
        // Deallocate some resources to simulate resource cycling
        if (frame % 10 == 0 && !allocations.empty()) {
            mm.Deallocate(allocations.back());
            allocations.pop_back();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    
    result.averageTimeMs = duration.count() / (60 * typicalAllocations.size());
    
    // Cleanup
    for (void* ptr : allocations) {
        mm.Deallocate(ptr);
    }
    
    return result;
}

void MemoryBenchmark::SimulateMemoryAccess(void* ptr, size_t size) {
    static std::atomic<size_t> accessCount{0};
    static std::chrono::high_resolution_clock::time_point lastBurst;
    
    auto now = std::chrono::high_resolution_clock::now();
    auto timeSinceLastBurst = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastBurst).count();
    
    // Track access patterns without actually accessing memory
    accessCount++;
    
    if (timeSinceLastBurst < 100 && accessCount > 1000) {  // Burst detection
        // Log suspicious burst access
        AccessPattern pattern;
        pattern.address = ptr;
        pattern.accessCount = accessCount;
        pattern.timestamp = now;
        pattern.isBurst = true;
        
        s_AccessPatterns.push_back(pattern);
        
        lastBurst = now;
        accessCount = 0;
    }
}

BenchmarkResult MemoryBenchmark::AnalyzeAccessPatterns() {
    BenchmarkResult result;
    result.name = "Access Pattern Analysis";
    
    std::unordered_map<void*, size_t> accessFrequency;
    
    // Analyze patterns without impacting performance
    for (const auto& pattern : s_AccessPatterns) {
        if (pattern.isBurst) {
            result.accessMetrics.suspiciousPatterns++;
        }
        
        accessFrequency[pattern.address]++;
    }
    
    // Find hotspots (limited to maxHotSpots for performance)
    std::vector<std::pair<void*, size_t>> sortedAccesses(
        accessFrequency.begin(), accessFrequency.end());
    
    std::partial_sort(sortedAccesses.begin(), 
                     sortedAccesses.begin() + std::min(accessFrequency.size(), 
                     (size_t)config.patternConfig.maxHotSpots),
                     sortedAccesses.end(),
                     [](const auto& a, const auto& b) {
                         return a.second > b.second;
                     });
    
    // Record hotspots
    for (size_t i = 0; i < std::min(sortedAccesses.size(), 
         (size_t)config.patternConfig.maxHotSpots); ++i) {
        AccessPatternMetrics::HotSpot hotspot;
        hotspot.address = sortedAccesses[i].first;
        hotspot.accessCount = sortedAccesses[i].second;
        hotspot.accessFrequency = hotspot.accessCount / 
            (double)result.totalTimeMs * 1000.0;
        
        result.accessMetrics.hotSpots.push_back(hotspot);
    }
    
    return result;
}

// Usage example:
void PrintBenchmarkResults() {
    MemoryBenchmark::BenchmarkConfig config;
    config.iterationCount = 10000;
    config.includeSecurityTests = true;
    config.includeNetworkTests = true;
    
    auto results = MemoryBenchmark::RunBenchmarks(config);
    
    for (const auto& result : results) {
        printf("Benchmark: %s\n", result.name.c_str());
        printf("  Average Time: %.3f ms\n", result.averageTimeMs);
        printf("  Throughput: %.2f MB/s\n", result.throughputMBps);
        printf("  Memory Overhead: %.2f%%\n", result.overheadPercent);
        printf("  Min/Max Time: %.3f/%.3f ms\n", result.minTimeMs, result.maxTimeMs);
        printf("\n");
    }
} 