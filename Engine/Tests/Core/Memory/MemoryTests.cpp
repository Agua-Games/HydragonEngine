#include "Core/Memory/MemorySystem.h"
#include "gtest/gtest.h"

class MemoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        Hydragon::Memory::MemorySystem::Get().initialize();
    }
    
    void TearDown() override {
        Hydragon::Memory::MemorySystem::Get().shutdown();
    }
};

TEST_F(MemoryTest, BasicAllocation) {
    void* ptr = Hydragon::Memory::MemorySystem::Get().allocate(1024);
    ASSERT_NE(ptr, nullptr);
    Hydragon::Memory::MemorySystem::Get().deallocate(ptr);
}

TEST_F(MemoryTest, ConcurrentAllocation) {
    constexpr size_t NUM_THREADS = 8;
    constexpr size_t ALLOCS_PER_THREAD = 1000;
    
    std::vector<std::thread> threads;
    std::atomic<size_t> successCount{0};
    
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&]() {
            for (size_t j = 0; j < ALLOCS_PER_THREAD; ++j) {
                void* ptr = Hydragon::Memory::MemorySystem::Get().allocate(16);
                if (ptr) {
                    successCount++;
                    Hydragon::Memory::MemorySystem::Get().deallocate(ptr);
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_EQ(successCount, NUM_THREADS * ALLOCS_PER_THREAD);
}

TEST_F(MemoryTest, AlignmentRequirements) {
    constexpr size_t alignment = 16;
    void* ptr = Hydragon::Memory::MemorySystem::Get().allocate(1024, 
        {.alignment = alignment});
    
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) & (alignment - 1), 0);
    
    Hydragon::Memory::MemorySystem::Get().deallocate(ptr);
}

TEST_F(MemoryTest, StressTest) {
    constexpr size_t NUM_ITERATIONS = 10000;
    std::vector<void*> allocations;
    allocations.reserve(NUM_ITERATIONS);
    
    // Random allocation sizes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 4096);
    
    for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
        size_t size = dis(gen);
        void* ptr = Hydragon::Memory::MemorySystem::Get().allocate(size);
        ASSERT_NE(ptr, nullptr);
        allocations.push_back(ptr);
        
        // Randomly deallocate some allocations
        if (i % 3 == 0 && !allocations.empty()) {
            size_t index = dis(gen) % allocations.size();
            Hydragon::Memory::MemorySystem::Get().deallocate(allocations[index]);
            allocations.erase(allocations.begin() + index);
        }
    }
    
    // Cleanup remaining allocations
    for (void* ptr : allocations) {
        Hydragon::Memory::MemorySystem::Get().deallocate(ptr);
    }
}

TEST_F(MemoryTest, ThreadSafetyStress) {
    constexpr size_t NUM_THREADS = 16;
    constexpr size_t OPS_PER_THREAD = 10000;
    
    std::vector<std::thread> threads;
    std::atomic<size_t> totalOps{0};
    std::atomic<size_t> failedOps{0};
    
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&, i]() {
            std::vector<void*> threadAllocations;
            threadAllocations.reserve(OPS_PER_THREAD / 2);
            
            for (size_t j = 0; j < OPS_PER_THREAD; ++j) {
                if (j % 2 == 0 || threadAllocations.empty()) {
                    // Allocate
                    void* ptr = Hydragon::Memory::MemorySystem::Get().allocate(16);
                    if (ptr) {
                        threadAllocations.push_back(ptr);
                        totalOps.fetch_add(1, std::memory_order_relaxed);
                    } else {
                        failedOps.fetch_add(1, std::memory_order_relaxed);
                    }
                } else {
                    // Deallocate
                    size_t index = j % threadAllocations.size();
                    Hydragon::Memory::MemorySystem::Get().deallocate(
                        threadAllocations[index]);
                    threadAllocations.erase(threadAllocations.begin() + index);
                    totalOps.fetch_add(1, std::memory_order_relaxed);
                }
            }
            
            // Cleanup remaining allocations
            for (void* ptr : threadAllocations) {
                Hydragon::Memory::MemorySystem::Get().deallocate(ptr);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_GT(totalOps, 0);
    EXPECT_LT(failedOps, totalOps * 0.01);  // Less than 1% failure rate
} 