/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Plugin {

using PluginHandle = uint32_t;
using PluginVersion = uint32_t;

struct Version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    
    bool operator>=(const Version& other) const {
        return (major > other.major) || 
               (major == other.major && minor > other.minor) ||
               (major == other.major && minor == other.minor && patch >= other.patch);
    }
};

struct PluginInfo {
    std::string name;
    std::string description;
    std::string author;
    Version version;
    Version engineVersion;
    std::vector<std::string> dependencies;
    std::vector<std::string> interfaces;
    bool sandboxed;
};

struct PluginStats {
    uint32_t loadedPlugins;
    uint32_t enabledPlugins;
    uint32_t failedLoads;
    uint32_t hotReloads;
    size_t totalMemoryUsed;
    float averageLoadTimeMs;
};

using InitPluginFunc = bool (*)(const PluginInfo*);
using ShutdownPluginFunc = void (*)();
using UpdatePluginFunc = void (*)(float deltaTime);

} // namespace Hydragon::Plugin 