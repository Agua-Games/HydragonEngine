#include "MemorySystem.h"
#include "Management/MemoryManager.h"
#include "Profiling/MemoryProfiler.h"
#include "Security/MemorySecurity.h"

namespace Hydragon {

std::unique_ptr<MemorySystem> MemorySystem::s_Instance;

void MemorySystem::Initialize(const Config& config) {
    s_Instance = std::make_unique<MemorySystem>();
    
    // Initialize subsystems
    s_Instance->m_Manager = std::make_unique<Memory::MemoryManager>();
    
    if (config.enableProfiling) {
        s_Instance->m_Profiler = std::make_unique<Memory::MemoryProfiler>();
    }
    
    if (config.enableSecurity) {
        s_Instance->m_Security = std::make_unique<Memory::MemorySecurity>();
    }
}

void* MemorySystem::Allocate(size_t size, size_t alignment) {
    void* ptr = GetManager().Allocate(size, alignment);
    
    if (s_Instance->m_Security) {
        s_Instance->m_Security->OnAllocation(ptr, size);
    }
    
    if (s_Instance->m_Profiler) {
        s_Instance->m_Profiler->TrackAllocation(ptr, size);
    }
    
    return ptr;
}

void MemorySystem::Shutdown() {
    if (s_Instance) {
        // Ensure proper cleanup order
        if (s_Instance->m_Profiler) {
            s_Instance->m_Profiler->EndCapture();
            s_Instance->m_Profiler.reset();
        }
        
        if (s_Instance->m_Security) {
            s_Instance->m_Security->ValidateAllMemory();
            s_Instance->m_Security.reset();
        }
        
        s_Instance->m_Manager.reset();
        s_Instance.reset();
    }
}

void MemorySystem::BeginMemoryCapture(const char* tag) {
    if (s_Instance && s_Instance->m_Profiler) {
        s_Instance->m_Profiler->BeginCapture(tag);
    }
}

void MemorySystem::EndMemoryCapture() {
    if (s_Instance && s_Instance->m_Profiler) {
        s_Instance->m_Profiler->EndCapture();
        
        // Auto-generate report if enabled
        if (s_Instance->m_Config.autoGenerateReports) {
            s_Instance->m_Profiler->GenerateReport("memory_capture.html");
        }
    }
}

bool MemorySystem::ValidateMemoryAccess(void* ptr, size_t size) {
    if (!s_Instance || !s_Instance->m_Security) return true;
    return s_Instance->m_Security->ValidateMemory(ptr, size);
}

// ... (implement other interface methods)

} // namespace Hydragon 