/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime serialization and data persistence
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeSerializationConfig {
    bool enableCompression = true;
    bool enableEncryption = true;
    bool enableVersioning = true;
    bool enableDeltaEncoding = true;
    uint32_t compressionLevel = 6;
    uint32_t bufferSize = 64 * 1024;  // 64KB
    std::string saveDirectory = "Saves";
    std::string tempDirectory = "Temp";
};

class RuntimeSerializationSystem {
public:
    static RuntimeSerializationSystem& Get();
    
    void Initialize(const RuntimeSerializationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    SerializeHandle BeginSerialize(const std::string& identifier);
    void EndSerialize(SerializeHandle handle);
    
    DeserializeHandle BeginDeserialize(const std::string& identifier);
    void EndDeserialize(DeserializeHandle handle);
    
    void RegisterSerializer(const std::string& type, std::unique_ptr<ITypeSerializer> serializer);
    void UnregisterSerializer(const std::string& type);
    
    void SaveState(const std::string& name, const SerializationData& data);
    bool LoadState(const std::string& name, SerializationData& data);
    
    void ExportData(const std::string& path, const SerializationData& data);
    bool ImportData(const std::string& path, SerializationData& data);
    
    void CreateSnapshot(const std::string& name);
    void RestoreSnapshot(const std::string& name);
    void DeleteSnapshot(const std::string& name);
    
    const RuntimeSerializationStats& GetStats() const { return m_Stats; }

private:
    RuntimeSerializationSystem() = default;
    
    RuntimeSerializationConfig m_Config;
    RuntimeSerializationStats m_Stats;
    std::unordered_map<std::string, std::unique_ptr<ITypeSerializer>> m_Serializers;
    std::unordered_map<SerializeHandle, SerializationContext> m_SerializeContexts;
    std::unordered_map<DeserializeHandle, DeserializationContext> m_DeserializeContexts;
    std::unique_ptr<ISerializationProvider> m_Provider;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 