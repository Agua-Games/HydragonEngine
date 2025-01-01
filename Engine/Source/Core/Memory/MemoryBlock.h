/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory block for Hydragon
 */

#pragma once

namespace Hydragon {

// Forward declarations for memory strategies
class IMemoryStrategy;

struct MemoryBlock {
    void* data;
    size_t size;
    size_t alignment;
    IMemoryStrategy* strategy;  // For future expansion into different allocation strategies
    
    #if HD_DEBUG
    const char* allocFile;
    int allocLine;
    const char* allocTag;
    #endif
};

} // namespace Hydragon 