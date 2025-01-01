/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Interface for module-specific memory management
 */

#pragma once
#include "MemoryTypes.h"

namespace Hydragon {
namespace Memory {

// Interface for module-specific memory management
class IModuleMemoryManager {
public:
    virtual ~IModuleMemoryManager() = default;
    
    // Memory limits
    virtual void setMemoryLimit(size_t limit) = 0;
    virtual void setPriority(ModulePriority priority) = 0;
    
    // Memory operations
    virtual void* allocate(size_t size, const AllocationInfo& info) = 0;
    virtual void deallocate(void* ptr) = 0;
    
    // State
    virtual size_t getCurrentUsage() const = 0;
    virtual bool isUnderPressure() const = 0;
    
    // Events
    Signal<void(size_t)> onMemoryPressure;
    Signal<void(void*, size_t)> onAllocation;
};

}} // namespace Hydragon::Memory 