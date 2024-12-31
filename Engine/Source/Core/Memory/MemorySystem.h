#pragma once
#include "Management/MemoryManager.h"
#include "Profiling/MemoryProfiler.h"
#include "Security/MemorySecurity.h"
#include <memory>

namespace Hydragon {

class MemorySystem {
public:
    struct Config {
        bool enableSecurity = true;
        bool enableProfiling = true;
        bool enableNetworking = false;
        std::string configPath;  // For external configuration
    };

    // Initialization and shutdown
    static void Initialize(const Config& config = Config());
    static void Shutdown();
    
    // Core systems access
    static MemoryManager& GetManager() { return *s_Instance->m_Manager; }
    static MemoryProfiler& GetProfiler() { return *s_Instance->m_Profiler; }
    static MemorySecurity& GetSecurity() { return *s_Instance->m_Security; }
    
    // High-level memory operations
    static void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t));
    static void Deallocate(void* ptr);
    
    // Profiling shortcuts
    static void BeginMemoryCapture(const char* tag);
    static void EndMemoryCapture();
    
    // Security shortcuts
    static void EnableSecurityFeatures(bool enable);
    static bool ValidateMemoryAccess(void* ptr, size_t size);

private:
    MemorySystem() = default;
    static std::unique_ptr<MemorySystem> s_Instance;
    
    std::unique_ptr<MemoryManager> m_Manager;
    std::unique_ptr<MemoryProfiler> m_Profiler;
    std::unique_ptr<MemorySecurity> m_Security;
};

} // namespace Hydragon 