/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory guard for Hydragon
 */

#pragma once
#include <cstdint>

namespace Hydragon::Memory {

/**
 * Provides memory corruption detection through guard patterns
 */
class MemoryGuard {
public:
    static constexpr uint64_t HEAD_PATTERN = 0xDEADBEEFDEADBEEF;
    static constexpr uint64_t TAIL_PATTERN = 0xFEEDFACEFEEDFACE;
    static constexpr size_t GUARD_SIZE = sizeof(uint64_t);

    /**
     * Adds guard patterns around a memory block
     * @param ptr Pointer to memory block
     * @param size Size of memory block
     * @return Pointer to usable memory (after guard)
     * @throws std::bad_alloc if memory cannot be guarded
     */
    static void* addGuards(void* ptr, size_t size);

    /**
     * Validates guard patterns around a memory block
     * @param ptr Pointer to memory block
     * @param size Size of memory block
     * @return True if guards are intact
     */
    static bool validateGuards(void* ptr, size_t size);
};

} // namespace Hydragon::Memory 