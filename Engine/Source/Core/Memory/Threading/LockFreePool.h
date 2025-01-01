/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Lock-free memory pool for Hydragon
 */

#pragma once
#include <atomic>
#include <array>

namespace Hydragon {
namespace Memory {

template<size_t BlockSize, size_t BlockCount>
class LockFreePool {
public:
    LockFreePool() {
        // Pre-allocate all blocks
        char* memory = static_cast<char*>(Platform::AlignedAlloc(BlockSize * BlockCount, HD_CACHE_LINE));
        
        // Initialize free list
        for (size_t i = 0; i < BlockCount - 1; ++i) {
            void* current = memory + (i * BlockSize);
            void* next = memory + ((i + 1) * BlockSize);
            *static_cast<void**>(current) = next;
        }
        
        // Set last block's next pointer to nullptr
        *static_cast<void**>(memory + ((BlockCount - 1) * BlockSize)) = nullptr;
        
        m_Head.store(memory, std::memory_order_release);
    }

    void* allocate() {
        void* ptr;
        void* expected = m_Head.load(std::memory_order_acquire);
        do {
            if (!expected) return nullptr;  // Pool is empty
            ptr = expected;
            // Try to update head to next block
            void* next = *static_cast<void**>(ptr);
        } while (!m_Head.compare_exchange_weak(expected, next,
                                             std::memory_order_release,
                                             std::memory_order_acquire));
        return ptr;
    }

    void deallocate(void* ptr) {
        void* expected = m_Head.load(std::memory_order_acquire);
        do {
            *static_cast<void**>(ptr) = expected;
        } while (!m_Head.compare_exchange_weak(expected, ptr,
                                             std::memory_order_release,
                                             std::memory_order_acquire));
    }

private:
    std::atomic<void*> m_Head;
};

}} // namespace Hydragon::Memory 