/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scripting system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace Hydragon::Scripting {

using ScriptHandle = uint32_t;
using CoroutineHandle = uint32_t;

using ScriptValue = std::variant<
    bool,
    int32_t,
    float,
    std::string,
    std::vector<ScriptValue>
>;

struct ScriptDesc {
    std::string path;
    bool autoStart = false;
    bool persistent = false;
    std::vector<std::string> dependencies;
};

struct ScriptStats {
    uint32_t activeScripts;
    uint32_t activeCoroutines;
    uint32_t totalExecutions;
    float averageExecutionTimeMs;
    size_t memoryUsed;
    uint32_t gcCollections;
};

} // namespace Hydragon::Scripting 