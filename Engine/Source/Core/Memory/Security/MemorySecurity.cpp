/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory security for Hydragon
 */

#include "MemorySecurity.h"
#include <atomic>

namespace Hydragon {
namespace Memory {

class SecurityMonitor {
public:
    void RegisterAccess(void* ptr, size_t size) {
        auto now = std::chrono::steady_clock::now();
        AccessRecord record{ptr, size, now};
        
        // Check for suspicious patterns
        if (IsSuspiciousAccess(record)) {
            OnSuspiciousAccess(record);
        }
        
        UpdateAccessHistory(record);
    }

private:
    struct AccessRecord {
        void* ptr;
        size_t size;
        std::chrono::steady_clock::time_point timestamp;
    };
    
    static constexpr size_t MAX_HISTORY = 1000;
    std::vector<AccessRecord> m_AccessHistory;
    std::mutex m_Mutex;
};

void MemorySecurity::OnAllocation(void* ptr, size_t size) {
    if (!ptr || !size) return;
    
    // Add canary values
    if (m_SecurityConfig.enableCanaries) {
        auto canary = GenerateCanary();
        PlaceCanaries(ptr, size, canary);
    }
    
    // Register allocation
    m_Monitor.RegisterAccess(ptr, size);
}

}} // namespace Hydragon::Memory 