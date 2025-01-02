/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Implementation of attribute serialization methods
 */

#pragma once
#include "Core/Data/AttributeSerialization.h"
#include "Core/Data/AttributeTypeMapping.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <zlib.h>
#include <nlohmann/json.hpp>

namespace Hydragon::Data {

bool AttributeSerializer::SaveToFile(
    const std::string& filename,
    const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
    const std::vector<AttributeDesc>& descriptions) {
    
    try {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            return false;
        }

        auto start = std::chrono::high_resolution_clock::now();

        bool success = false;
        switch (m_Config.format) {
            case SerializationFormat::Binary:
                success = SaveBinary(file, attributes, descriptions);
                break;
            case SerializationFormat::JSON:
                success = SaveJSON(file, attributes, descriptions);
                break;
            default:
                return false;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        return success;
    }
    catch (const std::exception& e) {
        // Log error
        return false;
    }
}

bool AttributeSerializer::SaveBinary(
    std::ostream& stream,
    const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
    const std::vector<AttributeDesc>& descriptions) {
    
    // Write header
    SerializationHeader header;
    header.version = m_Config.version;
    header.format = SerializationFormat::Binary;
    header.compressed = m_Config.compression;
    header.attributeCount = attributes.size();
    
    if (!WriteHeader(stream)) {
        return false;
    }

    // Serialize data
    std::vector<uint8_t> data;
    for (size_t i = 0; i < attributes.size(); ++i) {
        const auto& attr = attributes[i];
        const auto& desc = descriptions[i];

        // Write attribute metadata
        SerializeValue(stream, desc.name);
        SerializeValue(stream, static_cast<uint32_t>(desc.type));
        SerializeValue(stream, static_cast<uint32_t>(desc.owner));
        
        // Write attribute data
        size_t size = attr->Size();
        SerializeValue(stream, size);
        
        const uint8_t* rawData = static_cast<const uint8_t*>(attr->GetRawData());
        size_t dataSize = size * GetValueTypeSize(desc.type);
        
        if (m_Config.compression) {
            std::vector<uint8_t> compressed;
            if (!CompressData({rawData, rawData + dataSize}, compressed)) {
                return false;
            }
            stream.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
        } else {
            stream.write(reinterpret_cast<const char*>(rawData), dataSize);
        }
    }

    // Update header with final size and checksum
    header.dataSize = stream.tellp() - sizeof(SerializationHeader);
    header.checksum = CalculateChecksum(data);
    
    // Rewrite header with final values
    stream.seekp(0);
    WriteHeader(stream);

    return true;
}

bool AttributeSerializer::LoadBinary(
    std::istream& stream,
    std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
    std::vector<AttributeDesc>& descriptions) {
    
    SerializationHeader header;
    if (!ReadHeader(stream)) {
        return false;
    }

    attributes.clear();
    descriptions.clear();
    attributes.reserve(header.attributeCount);
    descriptions.reserve(header.attributeCount);

    for (uint32_t i = 0; i < header.attributeCount; ++i) {
        AttributeDesc desc;
        uint32_t typeValue, ownerValue;
        
        // Read metadata
        DeserializeValue(stream, desc.name);
        DeserializeValue(stream, typeValue);
        DeserializeValue(stream, ownerValue);
        
        desc.type = static_cast<ValueType>(typeValue);
        desc.owner = static_cast<AttributeOwner>(ownerValue);
        
        // Read data
        size_t size;
        DeserializeValue(stream, size);
        
        auto storage = CreateStorage(desc.type, desc.owner);
        storage->Resize(size);
        
        size_t dataSize = size * GetValueTypeSize(desc.type);
        uint8_t* rawData = static_cast<uint8_t*>(storage->GetRawData());
        
        if (header.compressed) {
            std::vector<uint8_t> compressed(dataSize);
            stream.read(reinterpret_cast<char*>(compressed.data()), dataSize);
            
            std::vector<uint8_t> decompressed;
            if (!DecompressData(compressed, decompressed)) {
                return false;
            }
            std::memcpy(rawData, decompressed.data(), dataSize);
        } else {
            stream.read(reinterpret_cast<char*>(rawData), dataSize);
        }
        
        attributes.push_back(std::move(storage));
        descriptions.push_back(desc);
    }

    return true;
}

bool AttributeSerializer::CompressData(
    const std::vector<uint8_t>& input,
    std::vector<uint8_t>& output) const {
    
    output.resize(compressBound(input.size()));
    uLongf destLen = output.size();
    
    int result = compress2(
        output.data(),
        &destLen,
        input.data(),
        input.size(),
        Z_BEST_COMPRESSION
    );
    
    if (result != Z_OK) {
        return false;
    }
    
    output.resize(destLen);
    return true;
}

bool AttributeSerializer::DecompressData(
    const std::vector<uint8_t>& input,
    std::vector<uint8_t>& output) const {
    
    output.resize(input.size() * 4);  // Estimate decompressed size
    uLongf destLen = output.size();
    
    while (true) {
        int result = uncompress(
            output.data(),
            &destLen,
            input.data(),
            input.size()
        );
        
        if (result == Z_BUF_ERROR) {
            output.resize(output.size() * 2);
            destLen = output.size();
            continue;
        }
        
        if (result != Z_OK) {
            return false;
        }
        
        output.resize(destLen);
        return true;
    }
}

bool AttributeSerializer::SaveJSON(
    std::ostream& stream,
    const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
    const std::vector<AttributeDesc>& descriptions) {
    
    try {
        nlohmann::json root;
        
        // Write header
        root["version"] = m_Config.version;
        root["format"] = "JSON";
        root["compressed"] = m_Config.compression;
        root["attributeCount"] = attributes.size();
        
        // Write attributes array
        nlohmann::json attributesArray = nlohmann::json::array();
        
        for (size_t i = 0; i < attributes.size(); ++i) {
            const auto& attr = attributes[i];
            const auto& desc = descriptions[i];
            
            nlohmann::json attributeObj;
            attributeObj["name"] = desc.name;
            attributeObj["type"] = static_cast<int>(desc.type);
            attributeObj["owner"] = static_cast<int>(desc.owner);
            attributeObj["size"] = attr->Size();
            
            // Serialize attribute data based on type
            nlohmann::json dataArray = nlohmann::json::array();
            const void* rawData = attr->GetRawData();
            
            for (size_t j = 0; j < attr->Size(); ++j) {
                switch (desc.type) {
                    case ValueType::Boolean:
                        dataArray.push_back(static_cast<const bool*>(rawData)[j]);
                        break;
                    case ValueType::Integer:
                        dataArray.push_back(static_cast<const int32_t*>(rawData)[j]);
                        break;
                    case ValueType::Float:
                        dataArray.push_back(static_cast<const float*>(rawData)[j]);
                        break;
                    case ValueType::String: {
                        const auto& strings = *static_cast<const std::vector<std::string>*>(rawData);
                        dataArray.push_back(strings[j]);
                        break;
                    }
                    default:
                        throw std::runtime_error("Unsupported type for JSON serialization");
                }
            }
            
            attributeObj["data"] = dataArray;
            attributesArray.push_back(attributeObj);
        }
        
        root["attributes"] = attributesArray;
        
        // Write to stream with pretty formatting
        stream << root.dump(2);
        return true;
    }
    catch (const std::exception& e) {
        // Log error
        return false;
    }
}

bool AttributeSerializer::LoadJSON(
    std::istream& stream,
    std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
    std::vector<AttributeDesc>& descriptions) {
    
    try {
        // Parse JSON
        nlohmann::json root;
        stream >> root;
        
        // Validate header
        if (root["version"].get<uint32_t>() > m_Config.version) {
            throw std::runtime_error("Unsupported version");
        }
        
        // Read attributes
        const auto& attributesArray = root["attributes"];
        for (const auto& attributeObj : attributesArray) {
            AttributeDesc desc;
            desc.name = attributeObj["name"].get<std::string>();
            desc.type = static_cast<ValueType>(attributeObj["type"].get<int>());
            desc.owner = static_cast<AttributeOwner>(attributeObj["owner"].get<int>());
            
            size_t size = attributeObj["size"].get<size_t>();
            auto storage = CreateStorage(desc.type, desc.owner);
            storage->Resize(size);
            
            // Deserialize data based on type
            const auto& dataArray = attributeObj["data"];
            void* rawData = storage->GetRawData();
            
            for (size_t i = 0; i < size; ++i) {
                switch (desc.type) {
                    case ValueType::Boolean:
                        static_cast<bool*>(rawData)[i] = dataArray[i].get<bool>();
                        break;
                    case ValueType::Integer:
                        static_cast<int32_t*>(rawData)[i] = dataArray[i].get<int32_t>();
                        break;
                    case ValueType::Float:
                        static_cast<float*>(rawData)[i] = dataArray[i].get<float>();
                        break;
                    case ValueType::String: {
                        auto& strings = *static_cast<std::vector<std::string>*>(rawData);
                        strings[i] = dataArray[i].get<std::string>();
                        break;
                    }
                    default:
                        throw std::runtime_error("Unsupported type for JSON deserialization");
                }
            }
            
            attributes.push_back(std::move(storage));
            descriptions.push_back(desc);
        }
        
        return true;
    }
    catch (const std::exception& e) {
        // Log error
        return false;
    }
}

} // namespace Hydragon::Data 