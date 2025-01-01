/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory benchmarks for Hydragon
 */

#include "Core/Memory/MemorySystem.h"
#include "benchmark/benchmark.h"
#include <vector>
#include <thread>

namespace {

using namespace Hydragon::Memory;

// Single-threaded allocation benchmark
static void BM_SingleThreadAllocation(benchmark::State& state) {
    const size_t size = state.range(0);
    
    for (auto _ : state) {
        void* ptr = MemorySystem::Get().allocate(size);
        benchmark::DoNotOptimize(ptr);
        MemorySystem::Get().deallocate(ptr);
    }
    
    state.SetBytesProcessed(int64_t(state.iterations()) * size);
}

// Multi-threaded allocation benchmark
static void BM_MultiThreadAllocation(benchmark::State& state) {
    const size_t numThreads = state.range(0);
    const size_t size = 1024;  // Fixed size for multi-thread test
    
    for (auto _ : state) {
        std::vector<std::thread> threads;
        std::atomic<size_t> completedOps{0};
        
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([&]() {
                void* ptr = MemorySystem::Get().allocate(size);
                benchmark::DoNotOptimize(ptr);
                MemorySystem::Get().deallocate(ptr);
                completedOps.fetch_add(1, std::memory_order_relaxed);
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    }
    
    state.SetBytesProcessed(int64_t(state.iterations()) * numThreads * size);
}

// Memory fragmentation benchmark
static void BM_FragmentationStress(benchmark::State& state) {
    const size_t numAllocs = state.range(0);
    std::vector<void*> ptrs;
    ptrs.reserve(numAllocs);
    
    for (auto _ : state) {
        // Allocate with varying sizes
        for (size_t i = 0; i < numAllocs; ++i) {
            size_t size = 16 << (i % 8);  // Sizes from 16 to 2048
            ptrs.push_back(MemorySystem::Get().allocate(size));
        }
        
        // Free in random order
        std::random_shuffle(ptrs.begin(), ptrs.end());
        for (void* ptr : ptrs) {
            MemorySystem::Get().deallocate(ptr);
        }
        ptrs.clear();
    }
}

} // anonymous namespace

// Register benchmarks
BENCHMARK(BM_SingleThreadAllocation)
    ->Range(8, 8<<13)  // 8B to 64KB
    ->UseRealTime();

BENCHMARK(BM_MultiThreadAllocation)
    ->Range(1, 32)  // 1 to 32 threads
    ->UseRealTime();

BENCHMARK(BM_FragmentationStress)
    ->Range(1<<8, 1<<12)  // 256 to 4096 allocations
    ->UseRealTime(); 