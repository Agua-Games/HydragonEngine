#pragma once
#include <thread>

namespace Hydragon {
namespace Threading {

inline size_t GetCurrentThreadId() {
    static thread_local size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    return threadId;
}

}} // namespace Hydragon::Threading 