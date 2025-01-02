/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin loading and management
 */

#pragma once
#include "../PluginTypes.h"
#include <memory>
#include <string>

namespace Hydragon::Plugin {

class IPluginLoader {
public:
    virtual ~IPluginLoader() = default;
    virtual bool LoadPlugin(const std::string& path, Plugin& outPlugin) = 0;
    virtual bool UnloadPlugin(Plugin& plugin) = 0;
    virtual bool ReloadPlugin(Plugin& plugin) = 0;
    virtual void* GetFunction(const Plugin& plugin, const std::string& name) = 0;
};

class DynamicPluginLoader : public IPluginLoader {
public:
    struct LoaderConfig {
        bool validateSignature = true;
        bool enableCache = true;
        bool resolveSymbols = true;
        std::string searchPath;
    };
    
    explicit DynamicPluginLoader(const LoaderConfig& config = {});
    bool LoadPlugin(const std::string& path, Plugin& outPlugin) override;
    bool UnloadPlugin(Plugin& plugin) override;
    bool ReloadPlugin(Plugin& plugin) override;
    void* GetFunction(const Plugin& plugin, const std::string& name) override;

private:
    LoaderConfig m_Config;
    std::unique_ptr<IDynamicLibrary> m_Library;
};

class ScriptPluginLoader : public IPluginLoader {
public:
    struct ScriptConfig {
        bool enableSandbox = true;
        bool enableDebug = true;
        uint32_t maxStackSize = 1024 * 1024;  // 1MB
        std::string scriptEngine;
    };
    
    explicit ScriptPluginLoader(const ScriptConfig& config = {});
    bool LoadPlugin(const std::string& path, Plugin& outPlugin) override;
    bool UnloadPlugin(Plugin& plugin) override;
    bool ReloadPlugin(Plugin& plugin) override;
    void* GetFunction(const Plugin& plugin, const std::string& name) override;

private:
    ScriptConfig m_Config;
    std::unique_ptr<IScriptEngine> m_Engine;
};

} // namespace Hydragon::Plugin 