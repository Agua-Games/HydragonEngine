/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory guard for Hydragon
 */

#pragma once
#include <cstdint>

namespace Hydragon {
namespace Memory {

class MemoryGuard {
public:
    static constexpr uint64_t HEAD_PATTERN = 0xDEADBEEFDEADBEEF;
    static constexpr uint64_t TAIL_PATTERN = 0xFEEDFACEFEEDFACE;
    static constexpr size_t GUARD_SIZE = sizeof(uint64_t);

    static void* addGuards(void* ptr, size_t size) {
        auto* head = static_cast<uint64_t*>(ptr);
        *head = HEAD_PATTERN;
        
        auto* tail = reinterpret_cast<uint64_t*>(
            static_cast<char*>(ptr) + GUARD_SIZE + size);
        *tail = TAIL_PATTERN;
        
        return static_cast<char*>(ptr) + GUARD_SIZE;
    }

    static bool validateGuards(void* ptr, size_t size) {
        auto* head = reinterpret_cast<uint64_t*>(
            static_cast<char*>(ptr) - GUARD_SIZE);
        auto* tail = reinterpret_cast<uint64_t*>(
            static_cast<char*>(ptr) + size);
            
        return *head == HEAD_PATTERN && *tail == TAIL_PATTERN;
    }
};

}} // namespace Hydragon::Memory 