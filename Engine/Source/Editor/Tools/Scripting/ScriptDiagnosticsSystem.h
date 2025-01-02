/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Script diagnostics and error reporting system
 */

#pragma once
#include "ScriptTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct ScriptDiagnosticsConfig {
    bool enableLinting = true;
    bool enableTypeChecking = true;
    bool enableStyleChecking = true;
    bool enableMetricsCollection = true;
    uint32_t maxDiagnosticsPerFile = 1000;
    uint32_t updateInterval = 500;  // ms
    std::string rulesetPath;
};

class ScriptDiagnosticsSystem {
public:
    static ScriptDiagnosticsSystem& Get();
    
    void Initialize(const ScriptDiagnosticsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void AnalyzeScript(const std::string& path);
    void ClearDiagnostics(const std::string& path);
    
    void AddRule(const std::string& name, const DiagnosticRule& rule);
    void RemoveRule(const std::string& name);
    
    void SetSeverity(const std::string& ruleName, DiagnosticSeverity severity);
    
    std::vector<Diagnostic> GetDiagnostics(const std::string& path) const;
    const ScriptMetrics& GetMetrics(const std::string& path) const;
    
    const ScriptDiagnosticsStats& GetStats() const { return m_Stats; }

private:
    ScriptDiagnosticsSystem() = default;
    
    ScriptDiagnosticsConfig m_Config;
    ScriptDiagnosticsStats m_Stats;
    std::unique_ptr<IDiagnosticsEngine> m_Engine;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 