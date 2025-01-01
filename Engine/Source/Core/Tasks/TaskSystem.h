/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Task system for Hydragon
 */

#pragma once
#include "Core/Memory/MemorySystem.h"

namespace Hydragon {
namespace Tasks {

class TaskSystem {
public:
    struct TaskConfig {
        bool enableFibers = true;
        uint32_t workerThreadCount = 0;  // 0 = auto based on CPU
        size_t fiberStackSize = 64 * 1024;
    };

    // Support for both immediate and deferred execution
    template<typename T>
    TaskHandle<T> schedule(const TaskDesc& desc) {
        // Integration with memory system for task allocations
        auto* memory = Memory::MemorySystem::Get().allocateTaskMemory(
            calculateTaskMemory(desc));
        return createTask(desc, memory);
    }
};

}} // namespace Hydragon::Tasks 