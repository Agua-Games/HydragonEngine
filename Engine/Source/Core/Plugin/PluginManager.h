/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin management and dynamic loading
 */

#pragma once

namespace Hydragon {
namespace Plugin {

class PluginManager {
public:
    // Support for both native and scripted plugins
    struct PluginInfo {
        bool isNative;
        bool hasEditorExtensions;
        bool requiresRestart;
    };

    // Dynamic loading/unloading without engine restart
    bool loadPlugin(const char* path, const PluginInfo& info);
};

}} // namespace Hydragon::Plugin 