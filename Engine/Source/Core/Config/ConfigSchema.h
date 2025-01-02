/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Configuration schema definition and validation
 */

#pragma once
#include "Core/Config/ConfigTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Config {

struct FieldDesc {
    ValueType type = ValueType::Null;
    bool required = false;
    bool unique = false;
    Variant defaultValue;
    std::string description;
    std::vector<ValidationRule> rules;
};

struct SchemaDesc {
    bool enableInheritance = true;
    bool enableVersioning = true;
    bool strict = false;
    std::string version = "1.0";
    std::string description;
};

class ConfigSchema {
public:
    static SchemaHandle Create(const std::string& name, const SchemaDesc& desc = {});
    
    // Field management
    void AddField(const std::string& name, const FieldDesc& desc);
    void RemoveField(const std::string& name);
    void SetFieldRequired(const std::string& name, bool required);
    void SetFieldDefault(const std::string& name, const Variant& value);
    
    // Validation rules
    void AddValidationRule(const std::string& field, const ValidationRule& rule);
    void RemoveValidationRule(const std::string& field, RuleHandle rule);
    
    // Schema inheritance
    void InheritFrom(SchemaHandle parent);
    void RemoveInheritance();
    std::vector<SchemaHandle> GetInheritanceChain() const;
    
    // Schema validation
    ValidationResult ValidateConfig(ConfigHandle config) const;
    ValidationResult ValidateValue(const std::string& field, const Variant& value) const;
    bool IsValid() const;
    
    // Schema queries
    const FieldDesc* GetField(const std::string& name) const;
    std::vector<std::string> GetFieldNames() const;
    const SchemaDesc& GetDescription() const;
    SchemaStats GetStats() const;
    
    // Schema versioning
    void SetVersion(const std::string& version);
    std::string GetVersion() const;
    bool IsCompatibleWith(const ConfigSchema& other) const;
    
    // Debug utilities
    void DumpToFile(const std::string& path) const;
    std::string ToString() const;

private:
    SchemaDesc m_Desc;
    std::unordered_map<std::string, FieldDesc> m_Fields;
    std::vector<SchemaHandle> m_Parents;
    SchemaStats m_Stats;
    bool m_Valid = false;
};

} // namespace Hydragon::Config 