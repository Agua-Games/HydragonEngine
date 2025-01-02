/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory security for Hydragon
 */

#pragma once

// STL includes
#include <array>

// Local includes
#include "../Management/MemoryBlock.h"

namespace Hydragon::Memory {

/**
 * Provides security features for memory operations
 */
class MemorySecurity {
public:
    static constexpr size_t CANARY_SIZE = 8;
    using Canary = std::array<uint8_t, CANARY_SIZE>;
    
    /**
     * Validates memory block integrity
     * @param ptr Memory block to validate
     * @param size Size of memory block
     * @return True if memory is valid
     */
    bool validateMemory(void* ptr, size_t size);

    /**
     * Enables a security feature
     * @param feature Feature to enable
     * @return True if feature was enabled successfully
     */
    bool enableFeature(SecurityFeature feature);
    
    /**
     * Handles new memory allocation
     * @param ptr Allocated memory
     * @param size Size of allocation
     */
    void onAllocation(void* ptr, size_t size);

    /**
     * Handles memory deallocation
     * @param ptr Memory being freed
     */
    void onDeallocation(void* ptr);
    
private:
    // ... (existing security code)
};

} // namespace Hydragon::Memory 