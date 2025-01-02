/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset package system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Asset/AssetPackageSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AssetPackageBindingsConfig {
    bool enableCompression = true;
    bool enableEncryption = true;
    bool enableStreaming = true;
    bool enableCaching = true;
    uint32_t maxOpenPackages = 16;
    uint32_t maxStreamBuffers = 8;
    uint32_t packageCacheSize = 64 * 1024 * 1024;  // 64MB
    std::string packageCachePath = "Cache/Packages";
};

class AssetPackageBindings {
public:
    static AssetPackageBindings& Get();
    
    void Initialize(const AssetPackageBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Package management
    PackageHandle CreatePackage(const std::string& name, const PackageDesc& desc = {});
    void OpenPackage(const std::string& path);
    void ClosePackage(PackageHandle handle);
    void SavePackage(PackageHandle handle, const std::string& path);
    
    // Asset operations
    void AddAsset(PackageHandle package, AssetHandle asset);
    void RemoveAsset(PackageHandle package, AssetHandle asset);
    bool ContainsAsset(PackageHandle package, AssetHandle asset) const;
    std::vector<AssetHandle> GetPackageAssets(PackageHandle handle) const;
    
    // Package streaming
    void StreamPackage(const std::string& path, const StreamParams& params = {});
    void UnstreamPackage(PackageHandle handle);
    bool IsPackageStreaming(PackageHandle handle) const;
    float GetStreamProgress(PackageHandle handle) const;
    
    // Package compression
    void SetCompressionLevel(PackageHandle handle, CompressionLevel level);
    void CompressPackage(PackageHandle handle);
    void DecompressPackage(PackageHandle handle);
    
    // Package encryption
    void SetEncryptionKey(PackageHandle handle, const std::string& key);
    void EncryptPackage(PackageHandle handle);
    void DecryptPackage(PackageHandle handle);
    
    // Package queries
    PackageInfo GetPackageInfo(PackageHandle handle) const;
    std::vector<PackageHandle> GetOpenPackages() const;
    PackageStats GetPackageStats() const;
    
    // Package utilities
    void DefragmentPackage(PackageHandle handle);
    void ValidatePackage(PackageHandle handle);
    void OptimizePackage(PackageHandle handle);
    
    // Debug utilities
    void EnablePackageTracking(bool enable);
    void DumpPackageContents(PackageHandle handle, const std::string& path);
    void AnalyzePackageUsage(PackageHandle handle);
    
    const AssetPackageBindingsStats& GetStats() const { return m_Stats; }

private:
    AssetPackageBindings() = default;
    
    AssetPackageBindingsConfig m_Config;
    AssetPackageBindingsStats m_Stats;
    std::unordered_map<PackageHandle, PackageInstance> m_Packages;
    std::unordered_map<std::string, PackageHandle> m_PackageMap;
    std::unordered_map<PackageHandle, StreamInstance> m_StreamInstances;
    std::unique_ptr<Asset::IPackageProcessor> m_Processor;
    std::unique_ptr<Asset::IPackageCache> m_Cache;
    std::unique_ptr<Asset::IStreamManager> m_StreamManager;
    std::unique_ptr<Asset::ICompressionManager> m_CompressionManager;
    std::unique_ptr<Asset::IEncryptionManager> m_EncryptionManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 