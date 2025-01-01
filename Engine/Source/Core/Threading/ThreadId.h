/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Thread ID utilities for Hydragon
 */

#pragma once
#include <thread>

namespace Hydragon {
namespace Threading {

inline size_t GetCurrentThreadId() {
    static thread_local size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    return threadId;
}

}} // namespace Hydragon::Threading 