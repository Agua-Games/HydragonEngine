#include "ModuleMemoryManager.h"
#include "Core/Profiling/MemoryProfiler.h"

namespace Hydragon {
namespace Memory {

ModuleMemoryManager::ModuleMemoryManager(
    const std::string& moduleName, 
    IMemoryStrategy* strategy)
    : m_ModuleName(moduleName)
    , m_Strategy(strategy)
    , m_MemoryLimit(0)
    , m_CurrentUsage(0)
    , m_Priority(ModulePriority::Normal) {
    
    HD_ASSERT(strategy, "Invalid memory strategy");
}

void* ModuleMemoryManager::allocate(size_t size, const AllocationInfo& info) {
    HD_PROFILE_SCOPE("ModuleMemory::Allocate");
    
    // Check memory limit
    if (m_CurrentUsage + size > m_MemoryLimit) {
        if (!handleAllocationFailure(size)) {
            return nullptr;
        }
    }
    
    // Perform allocation
    void* ptr = m_Strategy->allocate(size);
    if (ptr) {
        m_CurrentUsage += size;
        onAllocation.emit(ptr, size);
        
        // Check pressure after allocation
        checkMemoryPressure();
    }
    
    return ptr;
}

bool ModuleMemoryManager::handleAllocationFailure(size_t size) {
    // Emit pressure event to allow other modules to free memory
    onMemoryPressure.emit(size);
    
    // Wait briefly for memory to be freed
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    // Check if we can now allocate
    return m_CurrentUsage + size <= m_MemoryLimit;
}

void ModuleMemoryManager::checkMemoryPressure() {
    constexpr float PressureThreshold = 0.9f; // 90% usage
    
    if (m_MemoryLimit > 0) {
        float usage = static_cast<float>(m_CurrentUsage) / m_MemoryLimit;
        if (usage > PressureThreshold) {
            onMemoryPressure.emit(0); // Signal pressure without specific size
        }
    }
}

}} // namespace Hydragon::Memory 