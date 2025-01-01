#pragma once
#include "Types/MemoryTypes.h"
#include "Types/MemoryBlock.h"
#include "Management/IMemoryStrategy.h"

namespace Hydragon {
namespace Memory {

class MemoryManager {
public:
    static MemoryManager& Get();

    // Core allocation interface
    void* allocate(size_t size, const AllocationInfo& info = {});
    void deallocate(void* ptr);
    
    // Strategy management
    void setStrategy(IMemoryStrategy* strategy);
    IMemoryStrategy* getStrategy() const { return m_Strategy; }
    
    // Memory tracking
    MemoryStats getStats() const;
    const MemoryBlock* getAllocationInfo(void* ptr) const;

private:
    MemoryManager() = default;
    
    IMemoryStrategy* m_Strategy = nullptr;
    MemoryStats m_Stats;
    
    #if HD_DEBUG
        std::unordered_map<void*, MemoryBlock> m_Allocations;
    #endif
};

}} // namespace Hydragon::Memory 