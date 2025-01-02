/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core data system types and interfaces
 */

#pragma once
#include "Core/CoreTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Data {

// Forward declarations
class DataSystem;
class DataInstance;
struct DataSchema;

// Handle types
using DataHandle = Handle<DataInstance>;
using SchemaHandle = Handle<DataSchema>;
using ValidationHandle = Handle<ValidationResult>;
using SnapshotHandle = Handle<DataSnapshot>;

// Data value types
enum class ValueType {
    Null,
    Boolean,
    Integer,
    Float,
    String,
    Array,
    Object,
    Binary,
    Reference,
    Function
};

// Core interfaces
class IDataProcessor {
public:
    virtual ~IDataProcessor() = default;
    
    virtual bool ProcessData(DataHandle handle) = 0;
    virtual bool ValidateData(DataHandle handle) = 0;
    virtual void OptimizeData(DataHandle handle) = 0;
    virtual DataStats GetProcessingStats() const = 0;
};

class ISchemaValidator {
public:
    virtual ~ISchemaValidator() = default;
    
    virtual bool ValidateAgainstSchema(DataHandle data, const DataSchema& schema) = 0;
    virtual std::vector<SchemaError> GetValidationErrors() const = 0;
    virtual ValidationStats GetValidationStats() const = 0;
};

class IVersionController {
public:
    virtual ~IVersionController() = default;
    
    virtual SnapshotHandle CreateSnapshot(DataHandle handle, const std::string& desc) = 0;
    virtual bool RestoreSnapshot(DataHandle handle, SnapshotHandle snapshot) = 0;
    virtual std::vector<DataSnapshot> GetSnapshotHistory(DataHandle handle) const = 0;
};

class ICacheManager {
public:
    virtual ~ICacheManager() = default;
    
    virtual void EnableCaching(bool enable) = 0;
    virtual void SetCacheSize(uint32_t megabytes) = 0;
    virtual void ClearCache() = 0;
    virtual CacheStats GetCacheStats() const = 0;
};

class IIndexManager {
public:
    virtual ~IIndexManager() = default;
    
    virtual void CreateIndex(const std::string& field) = 0;
    virtual void RemoveIndex(const std::string& field) = 0;
    virtual bool HasIndex(const std::string& field) const = 0;
    virtual IndexStats GetIndexStats() const = 0;
};

// Statistics and metrics
struct DataStats {
    uint32_t totalInstances = 0;
    uint32_t activeInstances = 0;
    uint32_t totalSnapshots = 0;
    uint64_t totalMemoryUsage = 0;
    uint32_t validationErrors = 0;
    uint32_t cacheHits = 0;
    uint32_t cacheMisses = 0;
    float averageLoadTime = 0.0f;
    float averageQueryTime = 0.0f;
};

} // namespace Hydragon::Data 