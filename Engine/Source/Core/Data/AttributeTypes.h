/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Attribute system types and interfaces for procedural data management
 */

#pragma once
#include "Core/CoreTypes.h"
#include "Core/Data/DataTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Data {

// Attribute ownership levels (similar to Houdini)
enum class AttributeOwner {
    Detail,     // Single value for entire dataset
    Point,      // Per-point data
    Primitive,  // Per-primitive data (face, edge, etc.)
    Vertex,     // Per-vertex data
    Instance    // Per-instance data for instanced elements
};

// Attribute storage class
enum class AttributeClass {
    Uniform,    // Single value
    Varying,    // Interpolated values
    Indexed,    // Indexed lookup values
    Array,      // Array of values
    Dictionary  // Key-value pairs
};

// Attribute access patterns
enum class AttributeAccess {
    Read,
    Write,
    ReadWrite
};

struct AttributeDesc {
    std::string name;
    ValueType type = ValueType::Float;
    AttributeOwner owner = AttributeOwner::Point;
    AttributeClass storage = AttributeClass::Uniform;
    AttributeAccess access = AttributeAccess::ReadWrite;
    bool persistent = true;
    bool interpolatable = true;
    std::string defaultValue;
    std::string metadata;
};

// Interface for custom attribute handlers
class IAttributeHandler {
public:
    virtual ~IAttributeHandler() = default;
    
    virtual void OnAttributeCreated(const std::string& name, const AttributeDesc& desc) = 0;
    virtual void OnAttributeModified(const std::string& name, size_t index) = 0;
    virtual void OnAttributeDeleted(const std::string& name) = 0;
    virtual void OnAttributeRenamed(const std::string& oldName, const std::string& newName) = 0;
};

// Attribute group system (like Houdini groups)
struct GroupDesc {
    std::string name;
    AttributeOwner owner = AttributeOwner::Point;
    std::string pattern;  // Selection pattern
    bool persistent = true;
    std::string metadata;
};

class IGroupHandler {
public:
    virtual ~IGroupHandler() = default;
    
    virtual void OnGroupCreated(const std::string& name, const GroupDesc& desc) = 0;
    virtual void OnGroupModified(const std::string& name) = 0;
    virtual void OnGroupDeleted(const std::string& name) = 0;
    virtual void OnGroupMembership(const std::string& name, size_t index, bool added) = 0;
};

// Statistics for attribute system
struct AttributeStats {
    uint32_t totalAttributes = 0;
    uint32_t totalGroups = 0;
    uint32_t detailAttribs = 0;
    uint32_t pointAttribs = 0;
    uint32_t primAttribs = 0;
    uint32_t vertexAttribs = 0;
    uint32_t instanceAttribs = 0;
    uint64_t totalMemoryUsage = 0;
    float averageAccessTime = 0.0f;
};

// Attribute system configuration
struct AttributeSystemConfig {
    bool enableInterpolation = true;
    bool enableIndexing = true;
    bool enablePersistence = true;
    bool enableMetadata = true;
    uint32_t maxAttributes = 1000;
    uint32_t maxGroups = 100;
    uint32_t cacheSize = 64 * 1024 * 1024;  // 64MB
    std::string storageDirectory = "Data/Attributes";
};

} // namespace Hydragon::Data 