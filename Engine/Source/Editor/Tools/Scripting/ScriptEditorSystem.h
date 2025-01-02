/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Script editing and management system
 */

#pragma once
#include "ScriptTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct ScriptEditorConfig {
    bool enableAutoCompletion = true;
    bool enableSyntaxHighlighting = true;
    bool enableLiveCompilation = true;
    bool enableDebugger = true;
    uint32_t maxOpenScripts = 50;
    uint32_t undoLevels = 100;
    size_t maxFileSize = 1024 * 1024;  // 1MB
};

class ScriptEditorSystem {
public:
    static ScriptEditorSystem& Get();
    
    void Initialize(const ScriptEditorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void OpenScript(const std::string& path);
    void SaveScript(const std::string& path);
    void CloseScript(const std::string& path);
    
    void SetContent(const std::string& path, const std::string& content);
    std::string GetContent(const std::string& path) const;
    
    void CompileScript(const std::string& path);
    void DebugScript(const std::string& path);
    
    void AddBreakpoint(const std::string& path, uint32_t line);
    void RemoveBreakpoint(const std::string& path, uint32_t line);
    
    const ScriptEditorStats& GetStats() const { return m_Stats; }

private:
    ScriptEditorSystem() = default;
    
    ScriptEditorConfig m_Config;
    ScriptEditorStats m_Stats;
    std::unordered_map<std::string, ScriptDocument> m_Documents;
    std::unique_ptr<IScriptCompiler> m_Compiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 