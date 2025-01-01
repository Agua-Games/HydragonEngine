#pragma once
#include "Types/MemoryTypes.h"
#include "Management/IMemoryStrategy.h"

namespace Hydragon {
namespace Memory {

class MemorySystem {
public:
    static MemorySystem& Get();
    
    // Initialization
    void initialize(const MemoryConfig& config = {});
    void shutdown();
    
    // Core memory operations
    void* allocate(size_t size, const AllocationInfo& info = {});
    void deallocate(void* ptr);
    
    // Memory management
    void addStrategy(const std::string& name, IMemoryStrategy* strategy);
    void setDefaultStrategy(const std::string& name);
    
    // System state
    MemoryStats getSystemStats() const;
    bool isInitialized() const { return m_Initialized; }

private:
    MemorySystem() = default;
    
    bool m_Initialized = false;
    std::unordered_map<std::string, std::unique_ptr<IMemoryStrategy>> m_Strategies;
    IMemoryStrategy* m_DefaultStrategy = nullptr;
};

}} // namespace Hydragon::Memory 