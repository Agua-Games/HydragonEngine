/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Configuration validation system
 */

#pragma once
#include "Core/Config/ConfigTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Config {

enum class RuleSeverity {
    Info,
    Warning,
    Error,
    Critical
};

struct RuleCondition {
    std::string expression;
    std::string context;
    std::vector<std::string> dependencies;
};

struct RuleAction {
    std::string action;
    std::vector<std::string> parameters;
    bool autoFix = false;
};

struct RuleDesc {
    std::string name;
    std::string description;
    RuleSeverity severity = RuleSeverity::Error;
    bool enabled = true;
    bool inherited = false;
};

class ValidationRule {
public:
    static RuleHandle Create(const std::string& name, const RuleDesc& desc = {});
    
    // Rule definition
    void SetCondition(const RuleCondition& condition);
    void SetAction(const RuleAction& action);
    void SetSeverity(RuleSeverity severity);
    void SetEnabled(bool enabled);
    
    // Rule evaluation
    bool Evaluate(const ValidationContext& context) const;
    bool ApplyFix(ValidationContext& context) const;
    
    // Rule inheritance
    void InheritFrom(RuleHandle parent);
    void RemoveInheritance();
    std::vector<RuleHandle> GetInheritanceChain() const;
    
    // Rule queries
    const RuleDesc& GetDescription() const;
    bool IsEnabled() const;
    RuleStats GetStats() const;
    
private:
    RuleDesc m_Desc;
    RuleCondition m_Condition;
    RuleAction m_Action;
    std::vector<RuleHandle> m_Parents;
    RuleStats m_Stats;
    bool m_Valid = false;
};

class ValidationContext {
public:
    const Variant& GetValue() const;
    const std::string& GetPath() const;
    const ConfigSchema* GetSchema() const;
    const ValidationRule* GetRule() const;
    
    void SetValue(const Variant& value);
    void AddError(const ValidationError& error);
    void AddWarning(const ValidationError& warning);
    
private:
    Variant m_Value;
    std::string m_Path;
    const ConfigSchema* m_Schema = nullptr;
    const ValidationRule* m_Rule = nullptr;
    std::vector<ValidationError> m_Errors;
    std::vector<ValidationError> m_Warnings;
};

struct ValidationError {
    std::string message;
    std::string path;
    RuleSeverity severity;
    RuleHandle rule;
    bool canAutoFix = false;
};

struct ValidationResult {
    bool success = false;
    std::vector<ValidationError> errors;
    std::vector<ValidationError> warnings;
    ValidationStats stats;
};

struct ValidationStats {
    uint32_t totalRules = 0;
    uint32_t activeRules = 0;
    uint32_t totalValidations = 0;
    uint32_t failedValidations = 0;
    uint32_t autoFixCount = 0;
    float averageValidationTime = 0.0f;
};

} // namespace Hydragon::Config 