/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Configuration system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Config/ConfigSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct ConfigBindingsConfig {
    bool enableAutoSave = true;
    bool enableVersioning = true;
    bool enableEncryption = true;
    bool enableSchemaValidation = true;
    uint32_t maxConfigSize = 16 * 1024 * 1024;  // 16MB
    uint32_t maxHistoryEntries = 100;
    uint32_t autoSaveInterval = 300;  // 5 minutes
    std::string configRootPath = "Config";
};

class ConfigBindings {
public:
    static ConfigBindings& Get();
    
    void Initialize(const ConfigBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Config management
    ConfigHandle CreateConfig(const std::string& name, const ConfigDesc& desc = {});
    void LoadConfig(const std::string& path);
    void SaveConfig(ConfigHandle handle, const std::string& path = "");
    void CloseConfig(ConfigHandle handle);
    
    // Value operations
    void SetValue(ConfigHandle handle, const std::string& key, const Variant& value);
    Variant GetValue(ConfigHandle handle, const std::string& key) const;
    bool HasValue(ConfigHandle handle, const std::string& key) const;
    void RemoveValue(ConfigHandle handle, const std::string& key);
    
    // Section management
    void CreateSection(ConfigHandle handle, const std::string& section);
    void RemoveSection(ConfigHandle handle, const std::string& section);
    bool HasSection(ConfigHandle handle, const std::string& section) const;
    std::vector<std::string> GetSections(ConfigHandle handle) const;
    
    // Schema operations
    void SetSchema(ConfigHandle handle, const ConfigSchema& schema);
    bool ValidateSchema(ConfigHandle handle) const;
    std::vector<SchemaError> GetSchemaErrors(ConfigHandle handle) const;
    
    // Version control
    void CreateSnapshot(ConfigHandle handle, const std::string& description);
    void RestoreSnapshot(ConfigHandle handle, uint32_t version);
    std::vector<ConfigSnapshot> GetHistory(ConfigHandle handle) const;
    
    // Security
    void SetEncryptionKey(ConfigHandle handle, const std::string& key);
    void EncryptConfig(ConfigHandle handle);
    void DecryptConfig(ConfigHandle handle);
    
    // Import/Export
    void ImportJSON(ConfigHandle handle, const std::string& path);
    void ExportJSON(ConfigHandle handle, const std::string& path);
    void ImportYAML(ConfigHandle handle, const std::string& path);
    void ExportYAML(ConfigHandle handle, const std::string& path);
    
    // Config queries
    ConfigInfo GetConfigInfo(ConfigHandle handle) const;
    std::vector<ConfigHandle> GetLoadedConfigs() const;
    ConfigStats GetConfigStats() const;
    
    // Config utilities
    void ValidateConfig(ConfigHandle handle);
    void OptimizeConfig(ConfigHandle handle);
    void CompactConfig(ConfigHandle handle);
    
    // Auto-save management
    void EnableAutoSave(bool enable);
    void SetAutoSaveInterval(uint32_t seconds);
    void ForceAutoSave();
    
    // Debug utilities
    void EnableConfigTracking(bool enable);
    void DumpConfigState(ConfigHandle handle, const std::string& path);
    void AnalyzeConfigUsage(ConfigHandle handle);
    
    const ConfigBindingsStats& GetStats() const { return m_Stats; }

private:
    ConfigBindings() = default;
    
    ConfigBindingsConfig m_Config;
    ConfigBindingsStats m_Stats;
    std::unordered_map<ConfigHandle, ConfigInstance> m_Configs;
    std::unordered_map<std::string, ConfigHandle> m_ConfigMap;
    std::unordered_map<ConfigHandle, ConfigSchema> m_Schemas;
    std::unique_ptr<Config::IConfigProcessor> m_Processor;
    std::unique_ptr<Config::ISchemaValidator> m_SchemaValidator;
    std::unique_ptr<Config::IVersionController> m_VersionController;
    std::unique_ptr<Config::IEncryptionManager> m_EncryptionManager;
    std::unique_ptr<Config::IAutoSaveManager> m_AutoSaveManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 