/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core configuration system
 */

#pragma once
#include "Core/CoreTypes.h"
#include "Core/Memory/Memory.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Config {

// Forward declarations
class IConfigProcessor;
class ISchemaValidator;
class IVersionController;
class IEncryptionManager;
class IAutoSaveManager;

struct ConfigDesc {
    bool enableVersioning = true;
    bool enableEncryption = false;
    bool enableSchemaValidation = true;
    bool enableAutoSave = true;
    std::string format = "json";  // json, yaml, binary
};

class ConfigSystem {
public:
    static ConfigSystem& Get();
    
    bool Initialize();
    void Shutdown();
    
    void Update();
    
    // Core configuration operations
    ConfigHandle CreateConfig(const std::string& name, const ConfigDesc& desc = {});
    void DestroyConfig(ConfigHandle handle);
    bool LoadConfig(const std::string& path);
    bool SaveConfig(ConfigHandle handle, const std::string& path = "");
    
    // Value management
    void SetValue(ConfigHandle handle, const std::string& key, const Variant& value);
    Variant GetValue(ConfigHandle handle, const std::string& key) const;
    bool HasValue(ConfigHandle handle, const std::string& key) const;
    void RemoveValue(ConfigHandle handle, const std::string& key);
    
    // Section management
    void CreateSection(ConfigHandle handle, const std::string& section);
    void RemoveSection(ConfigHandle handle, const std::string& section);
    bool HasSection(ConfigHandle handle, const std::string& section) const;
    std::vector<std::string> GetSections(ConfigHandle handle) const;
    
    // Schema validation
    void SetSchema(ConfigHandle handle, const ConfigSchema& schema);
    bool ValidateSchema(ConfigHandle handle) const;
    std::vector<SchemaError> GetSchemaErrors(ConfigHandle handle) const;
    
    // Version control
    void CreateSnapshot(ConfigHandle handle, const std::string& description = "");
    void RestoreSnapshot(ConfigHandle handle, uint32_t version);
    std::vector<ConfigSnapshot> GetHistory(ConfigHandle handle) const;
    
    // Security
    void SetEncryptionKey(ConfigHandle handle, const std::string& key);
    void EncryptConfig(ConfigHandle handle);
    void DecryptConfig(ConfigHandle handle);
    
    // Utilities
    void ValidateConfig(ConfigHandle handle);
    void OptimizeConfig(ConfigHandle handle);
    void CompactConfig(ConfigHandle handle);
    
    // Debug
    void EnableConfigTracking(bool enable);
    void DumpConfigState(ConfigHandle handle, const std::string& path);
    void AnalyzeConfigUsage(ConfigHandle handle);
    
    const ConfigStats& GetStats() const { return m_Stats; }

private:
    ConfigSystem() = default;
    
    ConfigStats m_Stats;
    std::unordered_map<ConfigHandle, ConfigInstance> m_Configs;
    std::unique_ptr<IConfigProcessor> m_Processor;
    std::unique_ptr<ISchemaValidator> m_SchemaValidator;
    std::unique_ptr<IVersionController> m_VersionController;
    std::unique_ptr<IEncryptionManager> m_EncryptionManager;
    std::unique_ptr<IAutoSaveManager> m_AutoSaveManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Config 