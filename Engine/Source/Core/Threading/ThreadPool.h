/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Thread pool implementation for task execution
 */

#pragma once
#include "Core/Memory/Types/MemoryTypes.h"
#include "ThreadTypes.h"
#include <vector>
#include <queue>
#include <functional>

namespace Hydragon::Threading {

class ThreadPool {
public:
    struct ThreadPoolConfig {
        uint32_t threadCount = 0;     ///< 0 = Use hardware thread count
        size_t stackSize = 1024*1024; ///< Stack size per thread
        bool pinThreads = true;       ///< Pin threads to cores
        bool useAffinity = true;      ///< Use thread affinity masks
    };

    explicit ThreadPool(const ThreadPoolConfig& config = {});
    ~ThreadPool();

    void Initialize();
    void Shutdown();

    template<typename F>
    void EnqueueTask(F&& task, ThreadPriority priority = ThreadPriority::Normal);

    bool IsInitialized() const { return m_Initialized; }
    size_t GetActiveThreadCount() const;
    const ThreadStats& GetStats() const { return m_Stats; }

private:
    ThreadPoolConfig m_Config;
    bool m_Initialized = false;
    ThreadStats m_Stats;
    std::vector<ThreadHandle> m_Threads;
    std::queue<std::function<void()>> m_TaskQueue;
};

} // namespace Hydragon::Threading 