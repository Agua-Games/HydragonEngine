/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Configuration validation system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Config/ConfigValidationSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct ConfigValidationBindingsConfig {
    bool enableCustomRules = true;
    bool enableRuleInheritance = true;
    bool enableValidationCache = true;
    bool enableErrorReporting = true;
    uint32_t maxRulesPerConfig = 1000;
    uint32_t maxCustomRules = 100;
    uint32_t validationCacheSize = 4 * 1024 * 1024;  // 4MB
    std::string validationRulesPath = "Config/Rules";
};

class ConfigValidationBindings {
public:
    static ConfigValidationBindings& Get();
    
    void Initialize(const ConfigValidationBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Rule management
    RuleHandle CreateRule(const std::string& name, const RuleDesc& desc = {});
    void LoadRules(const std::string& path);
    void SaveRules(const std::string& path);
    void DeleteRule(RuleHandle handle);
    
    // Rule definition
    void SetRuleCondition(RuleHandle handle, const RuleCondition& condition);
    void SetRuleAction(RuleHandle handle, const RuleAction& action);
    void SetRuleSeverity(RuleHandle handle, RuleSeverity severity);
    void SetRuleEnabled(RuleHandle handle, bool enabled);
    
    // Custom rules
    void RegisterCustomRule(const std::string& name, std::unique_ptr<ICustomRule> rule);
    void UnregisterCustomRule(const std::string& name);
    
    // Validation operations
    ValidationResult ValidateConfig(ConfigHandle config);
    ValidationResult ValidateSection(ConfigHandle config, const std::string& section);
    ValidationResult ValidateValue(const Variant& value, const ValidationContext& context);
    
    // Error handling
    std::vector<ValidationError> GetValidationErrors(ValidationHandle handle) const;
    void ClearValidationErrors(ValidationHandle handle);
    ErrorStats GetErrorStats(ValidationHandle handle) const;
    
    // Rule queries
    RuleInfo GetRuleInfo(RuleHandle handle) const;
    std::vector<RuleHandle> GetActiveRules() const;
    ValidationStats GetValidationStats() const;
    
    // Debug utilities
    void EnableValidationTracking(bool enable);
    void DumpValidationState(const std::string& path);
    void AnalyzeRulePerformance();
    
    const ConfigValidationBindingsStats& GetStats() const { return m_Stats; }

private:
    ConfigValidationBindings() = default;
    
    ConfigValidationBindingsConfig m_Config;
    ConfigValidationBindingsStats m_Stats;
    std::unordered_map<RuleHandle, RuleInstance> m_Rules;
    std::unordered_map<std::string, RuleHandle> m_RuleMap;
    std::unordered_map<std::string, std::unique_ptr<ICustomRule>> m_CustomRules;
    std::unique_ptr<Config::IValidationProcessor> m_Processor;
    std::unique_ptr<Config::IValidationCache> m_Cache;
    std::unique_ptr<Config::IRuleEngine> m_RuleEngine;
    std::unique_ptr<Config::IErrorReporter> m_ErrorReporter;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 