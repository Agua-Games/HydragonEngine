/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory security for Hydragon
 */

#pragma once
#include "../Management/MemoryBlock.h"
#include <array>

namespace Hydragon {
namespace Memory {

class MemorySecurity {
public:
    // Moved from previous security features
    static constexpr size_t CANARY_SIZE = 8;
    using Canary = std::array<uint8_t, CANARY_SIZE>;
    
    // Core security interface
    bool ValidateMemory(void* ptr, size_t size);
    bool EnableFeature(SecurityFeature feature);
    
    // Security events
    void OnAllocation(void* ptr, size_t size);
    void OnDeallocation(void* ptr);
    
private:
    // ... (existing security code)
};

}} // namespace Hydragon::Memory 