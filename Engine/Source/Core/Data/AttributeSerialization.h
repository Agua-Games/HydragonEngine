/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Attribute serialization system for data persistence
 */

#pragma once
#include "Core/Data/AttributeTypes.h"
#include "Core/Data/AttributeStorage.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Data {

// Serialization format options
enum class SerializationFormat {
    Binary,     // Compact binary format
    JSON,       // Human-readable JSON
    Custom      // User-defined format
};

// Serialization configuration
struct SerializationConfig {
    SerializationFormat format = SerializationFormat::Binary;
    bool compression = true;
    bool includeMetadata = true;
    uint32_t version = 1;
    std::string customFormat;
};

class AttributeSerializer {
public:
    explicit AttributeSerializer(const SerializationConfig& config = {})
        : m_Config(config) {}

    // Main serialization interface
    bool SaveToFile(const std::string& filename, 
                   const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   const std::vector<AttributeDesc>& descriptions);

    bool LoadFromFile(const std::string& filename,
                     std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                     std::vector<AttributeDesc>& descriptions);

    // Utility functions
    static bool ValidateFormat(const std::string& filename);
    static uint32_t GetFormatVersion(const std::string& filename);
    static SerializationStats GetStats(const std::string& filename);

private:
    SerializationConfig m_Config;

    // Format-specific implementations
    bool SaveBinary(std::ostream& stream,
                   const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   const std::vector<AttributeDesc>& descriptions);

    bool LoadBinary(std::istream& stream,
                   std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   std::vector<AttributeDesc>& descriptions);

    bool SaveJSON(std::ostream& stream,
                 const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                 const std::vector<AttributeDesc>& descriptions);

    bool LoadJSON(std::istream& stream,
                 std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                 std::vector<AttributeDesc>& descriptions);

    // Helper functions
    bool WriteHeader(std::ostream& stream) const;
    bool ReadHeader(std::istream& stream) const;
    bool CompressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) const;
    bool DecompressData(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) const;
    
    template<typename T>
    bool SerializeValue(std::ostream& stream, const T& value);
    
    template<typename T>
    bool DeserializeValue(std::istream& stream, T& value);
};

// File format header
struct SerializationHeader {
    uint32_t magic = 0x48594441; // "HYDA"
    uint32_t version;
    SerializationFormat format;
    bool compressed;
    uint32_t attributeCount;
    uint64_t dataSize;
    uint32_t checksum;
};

// Statistics for serialization operations
struct SerializationStats {
    uint32_t totalAttributes = 0;
    uint32_t totalBytes = 0;
    float compressionRatio = 1.0f;
    float serializationTime = 0.0f;
    float deserializationTime = 0.0f;
    bool hasMetadata = false;
    uint32_t formatVersion = 0;
};

} // namespace Hydragon::Data 