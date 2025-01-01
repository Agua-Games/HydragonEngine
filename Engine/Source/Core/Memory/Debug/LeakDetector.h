/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory leak detection for Hydragon
 */

#pragma once
#include <unordered_map>
#include <stack>
#include "Core/Debug/Callstack.h"

namespace Hydragon {
namespace Memory {

class LeakDetector {
public:
    struct AllocationRecord {
        void* ptr;
        size_t size;
        const char* file;
        int line;
        uint64_t timestamp;
        Debug::Callstack callstack;
        std::thread::id threadId;
    };

    void onAllocation(void* ptr, size_t size) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        
        AllocationRecord record;
        record.ptr = ptr;
        record.size = size;
        record.timestamp = Time::GetTimestamp();
        record.threadId = std::this_thread::get_id();
        record.callstack = Debug::Callstack::Capture();
        
        m_ActiveAllocations[ptr] = record;
    }

    void onDeallocation(void* ptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_ActiveAllocations.erase(ptr);
    }

    void dumpLeaks(const char* filename) {
        std::ofstream report(filename);
        report << "Memory Leak Report - " << Time::GetTimeString() << "\n\n";

        size_t totalLeaks = 0;
        size_t totalBytes = 0;

        for (const auto& [ptr, record] : m_ActiveAllocations) {
            totalLeaks++;
            totalBytes += record.size;

            report << "Leak #" << totalLeaks << ":\n"
                  << "  Address: " << ptr << "\n"
                  << "  Size: " << record.size << " bytes\n"
                  << "  Allocated in: " << record.file << ":" << record.line << "\n"
                  << "  Thread: " << record.threadId << "\n"
                  << "  Callstack:\n" << record.callstack.toString() << "\n\n";
        }

        report << "Summary:\n"
               << "Total Leaks: " << totalLeaks << "\n"
               << "Total Bytes: " << totalBytes << "\n";
    }

private:
    std::mutex m_Mutex;
    std::unordered_map<void*, AllocationRecord> m_ActiveAllocations;
};

}} // namespace Hydragon::Memory 