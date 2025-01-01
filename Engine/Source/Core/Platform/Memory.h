/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Platform-specific memory management utilities
 */

#pragma once
#include <cstddef>

namespace Hydragon {
namespace Platform {

class Memory {
public:
    static void* AlignedAlloc(size_t size, size_t alignment);
    static void AlignedFree(void* ptr);
    static size_t GetPageSize();
    static bool IsPageAligned(void* ptr);
    
    // Platform-specific memory functions
    #if HD_PLATFORM_WINDOWS
        static void* VirtualAlloc(size_t size);
        static void VirtualFree(void* ptr);
    #else
        static void* MMap(size_t size);
        static void MUnmap(void* ptr, size_t size);
    #endif
};

}} // namespace Hydragon::Platform 