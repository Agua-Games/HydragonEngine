/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Script autocompletion and suggestion system
 */

#pragma once
#include "ScriptTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct ScriptCompletionConfig {
    bool enableIntelliSense = true;
    bool enableSnippets = true;
    bool enableDocumentation = true;
    bool enableFuzzyMatching = true;
    uint32_t maxSuggestions = 50;
    uint32_t cacheSize = 1000;
    float suggestionDelay = 0.2f;  // seconds
};

class ScriptCompletionSystem {
public:
    static ScriptCompletionSystem& Get();
    
    void Initialize(const ScriptCompletionConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RequestCompletion(const std::string& path, uint32_t line, uint32_t column);
    void CancelCompletion();
    
    void AddSnippet(const std::string& trigger, const ScriptSnippet& snippet);
    void RemoveSnippet(const std::string& trigger);
    
    void RegisterSymbol(const std::string& name, const SymbolInfo& info);
    void UnregisterSymbol(const std::string& name);
    
    CompletionList GetCompletions() const;
    Documentation GetDocumentation(const std::string& symbol) const;
    
    const ScriptCompletionStats& GetStats() const { return m_Stats; }

private:
    ScriptCompletionSystem() = default;
    
    ScriptCompletionConfig m_Config;
    ScriptCompletionStats m_Stats;
    std::unordered_map<std::string, SymbolInfo> m_Symbols;
    std::unique_ptr<ICompletionEngine> m_Engine;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 