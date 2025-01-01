#pragma once
#include <cstddef>
#include <unordered_map>
#include "MemorySecurity.h"
#include <functional>
#include "NetworkMemorySecurity.h"
#include "Types/MemoryBlock.h"
#include "Management/IMemoryStrategy.h"
#include "Management/StreamingMemoryManager.h"

namespace Hydragon {

struct MemoryBlock;
class IMemoryStrategy;

class MemoryManager {
public:
    static MemoryManager& Get() {
        static MemoryManager instance;
        return instance;
    }

    // Prevent copying
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    // Basic allocation/deallocation interface
    void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t));
    void Deallocate(void* ptr);

    // Memory tracking
    size_t GetTotalAllocated() const { return m_TotalAllocated; }
    size_t GetTotalFreed() const { return m_TotalFreed; }
    size_t GetCurrentUsage() const { return m_TotalAllocated - m_TotalFreed; }

    // Memory strategy interface
    void SetStrategy(IMemoryStrategy* strategy) { m_CurrentStrategy = strategy; }
    IMemoryStrategy* GetStrategy() const { return m_CurrentStrategy; }

    // Debug features
    #if HD_DEBUG
        void DumpMemoryStats() const;
        bool ValidateMemory() const;
        const MemoryBlock* GetAllocationInfo(void* ptr) const;
    #endif

    // Memory pools for common allocation sizes
    enum class PoolType {
        Small,    // For allocations <= 64 bytes
        Medium,   // For allocations <= 256 bytes
        Large     // For allocations > 256 bytes
    };
    
    // Get appropriate pool type for size
    static PoolType GetPoolType(size_t size) {
        if (size <= 64) return PoolType::Small;
        if (size <= 256) return PoolType::Medium;
        return PoolType::Large;
    }

    // Memory stats per pool
    struct PoolStats {
        size_t totalSize;
        size_t usedSize;
        size_t peakUsage;
        size_t allocationCount;
    };
    
    PoolStats GetPoolStats(PoolType pool) const;

    // Add security features
    struct SecurityConfig {
        bool enableCanaries = true;
        bool randomizePoolSizes = true;
        bool validateStrategies = true;
        bool enableNetworkSecurity = false;
    };
    
    void SetSecurityConfig(const SecurityConfig& config);
    
    #if HD_DEBUG
        struct MemoryWatchpoint {
            void* address;
            size_t size;
            std::function<void(const MemoryBlock&)> callback;
            MemorySecurity::Canary canary;
        };
        
        void AddWatchpoint(void* ptr, const MemoryWatchpoint& watchpoint);
        void RemoveWatchpoint(void* ptr);
        
        // Encryption key for debug data
        void SetDebugEncryptionKey(const std::array<uint8_t, 32>& key);
    #endif

    // Network security interface
    void EnableNetworkSecurity(bool enable) {
        m_SecurityConfig.enableNetworkSecurity = enable;
    }
    
    bool ValidateNetworkMemoryAccess(const void* ptr, size_t size) const;

private:
    MemoryManager() = default;
    
    size_t m_TotalAllocated = 0;
    size_t m_TotalFreed = 0;
    IMemoryStrategy* m_CurrentStrategy = nullptr;

    #if HD_DEBUG
        std::unordered_map<void*, MemoryBlock> m_AllocationMap;
    #endif

    SecurityConfig m_SecurityConfig;
    std::vector<std::pair<PoolType, size_t>> m_RandomizedPoolSizes;
    
    // Strategy validation
    bool ValidateStrategy(IMemoryStrategy* strategy) const;
    void* SecureAllocate(size_t size, size_t alignment);
    
    // Pool randomization
    void RandomizePoolSizes();
    
    #if HD_DEBUG
        std::unordered_map<void*, MemoryWatchpoint> m_Watchpoints;
        std::array<uint8_t, 32> m_DebugEncryptionKey;
    #endif

    NetworkMemorySecurity::NetworkSecurityConfig m_NetworkSecurityConfig;
};

} // namespace Hydragon 