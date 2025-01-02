/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Developer console and command system
 */

#pragma once
#include "../DebugTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::DevTools {

struct ConsoleConfig {
    bool enableHistory = true;
    bool enableAutoComplete = true;
    bool enableSuggestions = true;
    bool enableScripting = true;
    uint32_t maxHistoryEntries = 1000;
    uint32_t maxSuggestions = 10;
};

class ConsoleSystem {
public:
    static ConsoleSystem& Get();
    
    void Initialize(const ConsoleConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterCommand(const std::string& name, const CommandCallback& callback,
                        const std::string& description = "");
    void UnregisterCommand(const std::string& name);
    
    void ExecuteCommand(const std::string& command);
    void ExecuteScript(const std::string& path);
    
    void AddOutput(const std::string& text, OutputLevel level = OutputLevel::Info);
    void ClearOutput();
    
    std::vector<std::string> GetSuggestions(const std::string& partial) const;
    const CommandInfo* GetCommandInfo(const std::string& name) const;
    
    const ConsoleStats& GetStats() const { return m_Stats; }

private:
    ConsoleSystem() = default;
    
    ConsoleConfig m_Config;
    ConsoleStats m_Stats;
    std::vector<ConsoleCommand> m_Commands;
    std::vector<OutputEntry> m_Output;
    std::unique_ptr<IScriptEngine> m_ScriptEngine;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 