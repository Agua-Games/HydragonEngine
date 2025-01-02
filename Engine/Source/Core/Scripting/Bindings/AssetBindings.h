/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Asset/AssetSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AssetBindingsConfig {
    bool enableAsyncImport = true;
    bool enableVersioning = true;
    bool enableDependencyTracking = true;
    bool enableMetadata = true;
    uint32_t maxPendingImports = 100;
    uint32_t maxAssetRefs = 10000;
    uint32_t maxDependencies = 5000;
    std::string assetRegistryPath = "Data/AssetRegistry";
};

class AssetBindings {
public:
    static AssetBindings& Get();
    
    void Initialize(const AssetBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Asset management
    AssetHandle ImportAsset(const std::string& path, const ImportParams& params = {});
    void ExportAsset(AssetHandle handle, const std::string& path, const ExportParams& params = {});
    void DeleteAsset(AssetHandle handle);
    void RenameAsset(AssetHandle handle, const std::string& newName);
    
    // Asset references
    AssetRef CreateAssetRef(AssetHandle handle);
    void ReleaseAssetRef(AssetRef ref);
    AssetHandle GetAssetHandle(AssetRef ref) const;
    bool IsAssetLoaded(AssetHandle handle) const;
    
    // Asset metadata
    void SetMetadata(AssetHandle handle, const std::string& key, const Variant& value);
    Variant GetMetadata(AssetHandle handle, const std::string& key) const;
    void RemoveMetadata(AssetHandle handle, const std::string& key);
    
    // Asset dependencies
    void AddDependency(AssetHandle dependent, AssetHandle dependency);
    void RemoveDependency(AssetHandle dependent, AssetHandle dependency);
    std::vector<AssetHandle> GetDependencies(AssetHandle handle) const;
    std::vector<AssetHandle> GetDependents(AssetHandle handle) const;
    
    // Asset versioning
    void CreateVersion(AssetHandle handle, const std::string& version);
    void SwitchVersion(AssetHandle handle, const std::string& version);
    std::string GetCurrentVersion(AssetHandle handle) const;
    std::vector<std::string> GetVersionHistory(AssetHandle handle) const;
    
    // Asset queries
    std::vector<AssetHandle> FindAssets(const AssetQuery& query) const;
    AssetInfo GetAssetInfo(AssetHandle handle) const;
    AssetStats GetAssetStats() const;
    
    // Import/Export utilities
    void RegisterImporter(const std::string& extension, std::unique_ptr<IAssetImporter> importer);
    void RegisterExporter(const std::string& extension, std::unique_ptr<IAssetExporter> exporter);
    void SetImportCallback(const ImportCallback& callback);
    void SetExportCallback(const ExportCallback& callback);
    
    // Debug utilities
    void EnableAssetTracking(bool enable);
    void DumpAssetState(const std::string& path);
    void ValidateAssetReferences();
    
    const AssetBindingsStats& GetStats() const { return m_Stats; }

private:
    AssetBindings() = default;
    
    AssetBindingsConfig m_Config;
    AssetBindingsStats m_Stats;
    std::unordered_map<AssetHandle, AssetInstance> m_Assets;
    std::unordered_map<std::string, AssetHandle> m_AssetMap;
    std::unordered_map<AssetRef, AssetHandle> m_AssetRefs;
    std::unordered_map<AssetHandle, std::unordered_map<std::string, Variant>> m_Metadata;
    std::unique_ptr<Asset::IAssetProcessor> m_Processor;
    std::unique_ptr<Asset::IAssetRegistry> m_Registry;
    std::unique_ptr<Asset::IVersionController> m_VersionController;
    std::unique_ptr<Asset::IDependencyTracker> m_DependencyTracker;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 