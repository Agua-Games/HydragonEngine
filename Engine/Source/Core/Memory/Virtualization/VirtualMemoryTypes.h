/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Virtual memory type definitions
 */

#pragma once
#include <cstddef>
#include <cstdint>

namespace Hydragon::Memory {

enum class PageProtection {
    None,
    Read,
    ReadWrite,
    Execute,
    ReadExecute,
    ReadWriteExecute
};

struct VirtualMemoryStats {
    size_t totalReserved;
    size_t totalCommitted;
    size_t pageSize;
    size_t pageCount;
    uint32_t protectionFaults;
};

struct PageInfo {
    void* baseAddress;
    size_t size;
    PageProtection protection;
    bool isCommitted;
    bool isGuardPage;
};

struct VirtualRegion {
    void* startAddress;
    size_t size;
    std::vector<PageInfo> pages;
};

} // namespace Hydragon::Memory 