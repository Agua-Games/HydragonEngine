/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core scripting system
 */

#pragma once
#include "ScriptTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Scripting {

class ScriptSystem {
public:
    struct ScriptConfig {
        uint32_t maxScripts = 1000;
        uint32_t maxCoroutines = 100;
        bool enableHotReload = true;
        bool enableDebugger = true;
        bool enableProfiler = true;
        size_t memoryBudget = 128 * 1024 * 1024;  // 128MB
        std::string scriptRootPath;
    };

    static ScriptSystem& Get();
    
    void Initialize(const ScriptConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    
    ScriptHandle CreateScript(const ScriptDesc& desc);
    void DestroyScript(ScriptHandle handle);
    
    void ExecuteScript(ScriptHandle handle);
    void StopScript(ScriptHandle handle);
    
    void SetGlobal(const std::string& name, const ScriptValue& value);
    ScriptValue GetGlobal(const std::string& name) const;
    
    CoroutineHandle StartCoroutine(ScriptHandle script, const std::string& function);
    void StopCoroutine(CoroutineHandle handle);
    
    const ScriptStats& GetStats() const { return m_Stats; }

private:
    ScriptSystem() = default;
    
    ScriptConfig m_Config;
    ScriptStats m_Stats;
    std::unique_ptr<IScriptRuntime> m_Runtime;
    std::vector<Script> m_Scripts;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 