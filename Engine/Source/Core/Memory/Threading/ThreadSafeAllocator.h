/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Thread-safe memory allocation wrapper
 */

#pragma once
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <mutex>
#include <shared_mutex>

namespace Hydragon::Memory {

template<typename BaseAllocator>
class ThreadSafeAllocator : public IMemoryStrategy {
public:
    explicit ThreadSafeAllocator(const typename BaseAllocator::Config& config)
        : m_BaseAllocator(config) {}

    void* allocate(size_t size, const AllocationInfo& info) override {
        std::unique_lock<std::shared_mutex> lock(m_Mutex);
        return m_BaseAllocator.allocate(size, info);
    }

    void deallocate(void* ptr) override {
        std::unique_lock<std::shared_mutex> lock(m_Mutex);
        m_BaseAllocator.deallocate(ptr);
    }

    // Thread-safe statistics access
    size_t getTotalAllocated() const {
        std::shared_lock<std::shared_mutex> lock(m_Mutex);
        return m_BaseAllocator.getTotalAllocated();
    }

private:
    BaseAllocator m_BaseAllocator;
    mutable std::shared_mutex m_Mutex;
};

} // namespace Hydragon::Memory 