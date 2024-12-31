#include "MemoryManager.h"
#include "MemoryBlock.h"
#include <cstdlib>
#include <cassert>
#include <random>

namespace Hydragon {

void* MemoryManager::Allocate(size_t size, size_t alignment) {
    assert(size > 0 && "Allocation size must be greater than 0");
    
    // Basic aligned allocation for now
    void* ptr = nullptr;
    
    #if defined(_MSC_VER)
        ptr = _aligned_malloc(size, alignment);
    #else
        int result = posix_memalign(&ptr, alignment, size);
        if (result != 0) ptr = nullptr;
    #endif
    
    if (ptr) {
        m_TotalAllocated += size;
        
        #if HD_DEBUG
            // Store allocation info in debug map
            MemoryBlock block;
            block.data = ptr;
            block.size = size;
            block.alignment = alignment;
            m_AllocationMap[ptr] = block;
        #endif
    }
    
    return ptr;
}

void MemoryManager::Deallocate(void* ptr) {
    if (!ptr) return;
    
    #if HD_DEBUG
        auto it = m_AllocationMap.find(ptr);
        assert(it != m_AllocationMap.end() && "Attempting to free untracked memory!");
        if (it != m_AllocationMap.end()) {
            m_TotalFreed += it->second.size;
            m_AllocationMap.erase(it);
        }
    #endif
    
    #if defined(_MSC_VER)
        _aligned_free(ptr);
    #else
        free(ptr);
    #endif
}

void* MemoryManager::SecureAllocate(size_t size, size_t alignment) {
    // Add canary padding
    size_t totalSize = size;
    if (m_SecurityConfig.enableCanaries) {
        totalSize += 2 * MemorySecurity::CANARY_SIZE;
    }
    
    void* rawPtr = Allocate(totalSize, alignment);
    if (!rawPtr) return nullptr;
    
    if (m_SecurityConfig.enableCanaries) {
        auto canary = MemorySecurity::GenerateCanary();
        // Place canaries at start and end
        std::memcpy(rawPtr, canary.data(), MemorySecurity::CANARY_SIZE);
        std::memcpy(static_cast<char*>(rawPtr) + totalSize - MemorySecurity::CANARY_SIZE,
                   canary.data(), MemorySecurity::CANARY_SIZE);
        
        // Return pointer after first canary
        return static_cast<char*>(rawPtr) + MemorySecurity::CANARY_SIZE;
    }
    
    return rawPtr;
}

bool MemoryManager::ValidateStrategy(IMemoryStrategy* strategy) const {
    if (!strategy) return false;
    
    // Validate strategy vtable
    void** vtable = *reinterpret_cast<void***>(strategy);
    if (!vtable) return false;
    
    // Check for common vtable hijacking patterns
    // (This is a simplified example - real implementation would be more thorough)
    uintptr_t vtableAddr = reinterpret_cast<uintptr_t>(vtable);
    if (vtableAddr < 0x1000) return false;  // Suspicious low address
    
    return true;
}

void MemoryManager::RandomizePoolSizes() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Randomize pool size boundaries within reasonable ranges
    std::uniform_int_distribution<> small_dis(32, 96);
    std::uniform_int_distribution<> medium_dis(128, 384);
    
    m_RandomizedPoolSizes.clear();
    m_RandomizedPoolSizes.emplace_back(PoolType::Small, small_dis(gen));
    m_RandomizedPoolSizes.emplace_back(PoolType::Medium, medium_dis(gen));
}

void* MemoryManager::BaseAllocate(size_t size, size_t alignment) {
    // Inlined fast path for basic allocations
    #if defined(_MSC_VER)
        return _aligned_malloc(size, alignment);
    #else
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment, size) == 0)
            return ptr;
        return nullptr;
    #endif
}

// Cache security settings for faster checks
bool MemoryManager::NeedsSecurityChecks() const {
    static thread_local bool cached_result = false;
    static thread_local uint64_t cache_version = 0;
    
    if (cache_version != m_SecurityConfigVersion) {
        cached_result = m_SecurityConfig.enableCanaries || 
                       m_SecurityConfig.randomizePoolSizes ||
                       m_SecurityConfig.validateStrategies;
        cache_version = m_SecurityConfigVersion;
    }
    
    return cached_result;
}

} // namespace Hydragon 