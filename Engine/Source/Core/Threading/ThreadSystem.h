/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core threading system
 */

#pragma once
#include "ThreadTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Threading {

class ThreadSystem {
public:
    struct ThreadConfig {
        uint32_t maxThreads = 16;
        uint32_t maxFibers = 1000;
        bool enableWorkStealing = true;
        bool enableFiberPooling = true;
        bool enableThreadAffinity = true;
        size_t stackSize = 1024 * 1024;  // 1MB per thread
        size_t fiberStackSize = 64 * 1024;  // 64KB per fiber
    };

    static ThreadSystem& Get();
    
    void Initialize(const ThreadConfig& config = {});
    void Shutdown();
    
    ThreadHandle CreateThread(const ThreadDesc& desc);
    void DestroyThread(ThreadHandle handle);
    
    FiberHandle CreateFiber(const FiberDesc& desc);
    void DestroyFiber(FiberHandle handle);
    
    void SetThreadAffinity(ThreadHandle thread, uint32_t coreMask);
    void SetThreadPriority(ThreadHandle thread, ThreadPriority priority);
    
    void SwitchToFiber(FiberHandle fiber);
    void YieldFiber();
    
    const ThreadStats& GetStats() const { return m_Stats; }

private:
    ThreadSystem() = default;
    
    ThreadConfig m_Config;
    ThreadStats m_Stats;
    std::vector<Thread> m_Threads;
    std::vector<Fiber> m_Fibers;
    std::unique_ptr<IThreadScheduler> m_Scheduler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Threading 