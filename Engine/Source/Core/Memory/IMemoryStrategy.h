/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory strategy interface definitions
 */

#pragma once
#include "Types/MemoryTypes.h"

namespace Hydragon {
namespace Memory {

class IMemoryStrategy {
public:
    virtual ~IMemoryStrategy() = default;
    
    virtual void* allocate(size_t size, const AllocationInfo& info) = 0;
    virtual void deallocate(void* ptr) = 0;
    virtual bool owns(void* ptr) const = 0;
    
    // Memory stats
    virtual MemoryStats getStats() const = 0;
    
    // Optional: Strategy-specific operations
    virtual void compact() {}
    virtual void reset() {}
};

}} // namespace Hydragon::Memory 