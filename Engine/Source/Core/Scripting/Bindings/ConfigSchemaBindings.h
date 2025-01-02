/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Configuration schema system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Config/ConfigSchemaSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct ConfigSchemaBindingsConfig {
    bool enableSchemaInheritance = true;
    bool enableCustomValidators = true;
    bool enableSchemaVersioning = true;
    bool enableSchemaCaching = true;
    uint32_t maxSchemaSize = 1024 * 1024;  // 1MB
    uint32_t maxCustomValidators = 50;
    uint32_t schemaCacheSize = 8 * 1024 * 1024;  // 8MB
    std::string schemaRootPath = "Config/Schemas";
};

class ConfigSchemaBindings {
public:
    static ConfigSchemaBindings& Get();
    
    void Initialize(const ConfigSchemaBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Schema management
    SchemaHandle CreateSchema(const std::string& name, const SchemaDesc& desc = {});
    void LoadSchema(const std::string& path);
    void SaveSchema(SchemaHandle handle, const std::string& path);
    void CloseSchema(SchemaHandle handle);
    
    // Schema definition
    void AddField(SchemaHandle handle, const std::string& name, const FieldDesc& desc);
    void RemoveField(SchemaHandle handle, const std::string& name);
    void SetFieldRequired(SchemaHandle handle, const std::string& name, bool required);
    void SetFieldDefault(SchemaHandle handle, const std::string& name, const Variant& value);
    
    // Validation rules
    void AddValidationRule(SchemaHandle handle, const std::string& field, const ValidationRule& rule);
    void RemoveValidationRule(SchemaHandle handle, const std::string& field, RuleHandle rule);
    void AddCustomValidator(const std::string& name, std::unique_ptr<ICustomValidator> validator);
    
    // Schema inheritance
    void InheritSchema(SchemaHandle child, SchemaHandle parent);
    void RemoveInheritance(SchemaHandle child);
    std::vector<SchemaHandle> GetInheritanceChain(SchemaHandle handle) const;
    
    // Schema validation
    ValidationResult ValidateConfig(ConfigHandle config, SchemaHandle schema);
    ValidationResult ValidateValue(const Variant& value, const FieldDesc& desc);
    std::vector<ValidationError> GetValidationErrors(ValidationHandle handle) const;
    
    // Schema queries
    SchemaInfo GetSchemaInfo(SchemaHandle handle) const;
    std::vector<SchemaHandle> GetLoadedSchemas() const;
    SchemaStats GetSchemaStats() const;
    
    // Schema utilities
    void ValidateSchema(SchemaHandle handle);
    void OptimizeSchema(SchemaHandle handle);
    void MergeSchemas(SchemaHandle target, const std::vector<SchemaHandle>& sources);
    
    // Debug utilities
    void EnableSchemaTracking(bool enable);
    void DumpSchemaState(SchemaHandle handle, const std::string& path);
    void AnalyzeSchemaUsage(SchemaHandle handle);
    
    const ConfigSchemaBindingsStats& GetStats() const { return m_Stats; }

private:
    ConfigSchemaBindings() = default;
    
    ConfigSchemaBindingsConfig m_Config;
    ConfigSchemaBindingsStats m_Stats;
    std::unordered_map<SchemaHandle, SchemaInstance> m_Schemas;
    std::unordered_map<std::string, SchemaHandle> m_SchemaMap;
    std::unordered_map<std::string, std::unique_ptr<ICustomValidator>> m_CustomValidators;
    std::unique_ptr<Config::ISchemaProcessor> m_Processor;
    std::unique_ptr<Config::ISchemaCache> m_Cache;
    std::unique_ptr<Config::IValidationEngine> m_ValidationEngine;
    std::unique_ptr<Config::IInheritanceManager> m_InheritanceManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 