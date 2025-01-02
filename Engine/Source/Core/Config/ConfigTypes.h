/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core configuration system types and interfaces
 */

#pragma once
#include "Core/CoreTypes.h"
#include <string>
#include <vector>
#include <memory>

namespace Hydragon::Config {

// Forward declarations
class ConfigSystem;
class ConfigInstance;
struct ConfigSchema;

// Handle types
using ConfigHandle = Handle<ConfigInstance>;
using SchemaHandle = Handle<ConfigSchema>;
using ValidationHandle = Handle<ValidationResult>;
using SnapshotHandle = Handle<ConfigSnapshot>;

// Configuration value types
enum class ValueType {
    Null,
    Boolean,
    Integer,
    Float,
    String,
    Array,
    Object,
    Binary
};

// Configuration interfaces
class IConfigProcessor {
public:
    virtual ~IConfigProcessor() = default;
    
    virtual bool ProcessConfig(ConfigHandle handle) = 0;
    virtual bool ValidateConfig(ConfigHandle handle) = 0;
    virtual void OptimizeConfig(ConfigHandle handle) = 0;
    virtual ConfigStats GetProcessingStats() const = 0;
};

class ISchemaValidator {
public:
    virtual ~ISchemaValidator() = default;
    
    virtual bool ValidateAgainstSchema(ConfigHandle config, const ConfigSchema& schema) = 0;
    virtual std::vector<SchemaError> GetValidationErrors() const = 0;
    virtual ValidationStats GetValidationStats() const = 0;
};

class IVersionController {
public:
    virtual ~IVersionController() = default;
    
    virtual SnapshotHandle CreateSnapshot(ConfigHandle handle, const std::string& desc) = 0;
    virtual bool RestoreSnapshot(ConfigHandle handle, SnapshotHandle snapshot) = 0;
    virtual std::vector<ConfigSnapshot> GetSnapshotHistory(ConfigHandle handle) const = 0;
};

class IEncryptionManager {
public:
    virtual ~IEncryptionManager() = default;
    
    virtual bool SetEncryptionKey(ConfigHandle handle, const std::string& key) = 0;
    virtual bool EncryptConfig(ConfigHandle handle) = 0;
    virtual bool DecryptConfig(ConfigHandle handle) = 0;
    virtual bool IsEncrypted(ConfigHandle handle) const = 0;
};

class IAutoSaveManager {
public:
    virtual ~IAutoSaveManager() = default;
    
    virtual void EnableAutoSave(bool enable) = 0;
    virtual void SetAutoSaveInterval(uint32_t seconds) = 0;
    virtual void ForceAutoSave() = 0;
    virtual bool IsAutoSaveEnabled() const = 0;
};

// Statistics and metrics
struct ConfigStats {
    uint32_t totalConfigs = 0;
    uint32_t activeConfigs = 0;
    uint32_t totalSnapshots = 0;
    uint64_t totalMemoryUsage = 0;
    uint32_t validationErrors = 0;
    uint32_t autoSaveCount = 0;
    float averageLoadTime = 0.0f;
    float averageSaveTime = 0.0f;
};

} // namespace Hydragon::Config 