/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Concurrent memory allocator for Hydragon
 */

#include "ConcurrentAllocator.h"
#include "Core/Platform/Memory.h"
#include "Core/Threading/ThreadId.h"

namespace Hydragon {
namespace Memory {

thread_local ThreadLocalCache* ThreadLocalCache::current = nullptr;

void* ConcurrentAllocator::allocate(size_t size, const AllocationInfo& info) {
    // Fast path: Try thread-local cache first
    if (m_Config.enableThreadCache && size <= ThreadLocalCache::BUCKET_SIZE) {
        if (void* ptr = allocateFromCache(size)) {
            return ptr;
        }
    }

    // Medium path: Try central buffers
    if (size <= CENTRAL_BUFFER_SIZE) {
        if (void* ptr = allocateFromCentral(size)) {
            return ptr;
        }
    }

    // Slow path: Fall back to system allocation
    return Platform::AlignedAlloc(size, HD_CACHE_LINE);
}

void* ConcurrentAllocator::allocateFromCache(size_t size) {
    auto* cache = ThreadLocalCache::current;
    if (!cache) {
        // Initialize thread cache on first use
        size_t threadId = Threading::GetCurrentThreadId();
        if (threadId < m_Config.maxThreads) {
            cache = &m_ThreadCaches[threadId];
            ThreadLocalCache::current = cache;
        } else {
            return nullptr;
        }
    }

    // Find appropriate bucket
    size_t bucketIndex = getBucketIndex(size);
    auto& bucket = cache->buckets[bucketIndex];

    // Try to pop from bucket
    void* ptr = nullptr;
    void* expected = bucket.head.load(std::memory_order_relaxed);
    do {
        if (!expected) return nullptr;
        ptr = expected;
        // Try to update head to next block
        void* next = *static_cast<void**>(ptr);
    } while (!bucket.head.compare_exchange_weak(expected, next,
                                              std::memory_order_release,
                                              std::memory_order_relaxed));

    bucket.count.fetch_sub(1, std::memory_order_relaxed);
    return ptr;
}

bool ConcurrentAllocator::returnToCache(void* ptr, size_t size) {
    auto* cache = ThreadLocalCache::current;
    if (!cache) return false;

    size_t bucketIndex = getBucketIndex(size);
    auto& bucket = cache->buckets[bucketIndex];

    // Check if bucket is full
    if (bucket.count.load(std::memory_order_relaxed) >= ThreadLocalCache::BUCKET_SIZE) {
        return false;
    }

    // Push onto bucket
    void* expected = bucket.head.load(std::memory_order_relaxed);
    do {
        *static_cast<void**>(ptr) = expected;
    } while (!bucket.head.compare_exchange_weak(expected, ptr,
                                              std::memory_order_release,
                                              std::memory_order_relaxed));

    bucket.count.fetch_add(1, std::memory_order_relaxed);
    return true;
}
} // namespace Memory
} // namespace Hydragon 