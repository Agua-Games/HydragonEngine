/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset bundle system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Asset/AssetBundleSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AssetBundleBindingsConfig {
    bool enableDeltaBundles = true;
    bool enableBundlePatching = true;
    bool enableBundleValidation = true;
    bool enableManifestCaching = true;
    uint32_t maxConcurrentBundles = 8;
    uint32_t maxPatchSize = 256 * 1024 * 1024;  // 256MB
    uint32_t manifestCacheSize = 16 * 1024 * 1024;  // 16MB
    std::string bundleCachePath = "Cache/Bundles";
};

class AssetBundleBindings {
public:
    static AssetBundleBindings& Get();
    
    void Initialize(const AssetBundleBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Bundle management
    BundleHandle CreateBundle(const std::string& name, const BundleDesc& desc = {});
    void LoadBundle(const std::string& path);
    void UnloadBundle(BundleHandle handle);
    void SaveBundle(BundleHandle handle, const std::string& path);
    
    // Asset operations
    void AddToBundle(BundleHandle bundle, AssetHandle asset);
    void RemoveFromBundle(BundleHandle bundle, AssetHandle asset);
    bool IsInBundle(BundleHandle bundle, AssetHandle asset) const;
    std::vector<AssetHandle> GetBundleAssets(BundleHandle handle) const;
    
    // Bundle patching
    void CreatePatch(BundleHandle source, BundleHandle target, const std::string& path);
    void ApplyPatch(BundleHandle handle, const std::string& patchPath);
    bool ValidatePatch(const std::string& patchPath) const;
    float GetPatchSize(const std::string& patchPath) const;
    
    // Bundle manifests
    void GenerateManifest(BundleHandle handle, const std::string& path);
    void LoadManifest(const std::string& path);
    bool ValidateManifest(BundleHandle handle) const;
    BundleManifest GetManifest(BundleHandle handle) const;
    
    // Bundle queries
    BundleInfo GetBundleInfo(BundleHandle handle) const;
    std::vector<BundleHandle> GetLoadedBundles() const;
    BundleStats GetBundleStats() const;
    
    // Bundle utilities
    void OptimizeBundle(BundleHandle handle);
    void ValidateBundle(BundleHandle handle);
    void RepairBundle(BundleHandle handle);
    
    // Debug utilities
    void EnableBundleTracking(bool enable);
    void DumpBundleContents(BundleHandle handle, const std::string& path);
    void AnalyzeBundleUsage(BundleHandle handle);
    
    const AssetBundleBindingsStats& GetStats() const { return m_Stats; }

private:
    AssetBundleBindings() = default;
    
    AssetBundleBindingsConfig m_Config;
    AssetBundleBindingsStats m_Stats;
    std::unordered_map<BundleHandle, BundleInstance> m_Bundles;
    std::unordered_map<std::string, BundleHandle> m_BundleMap;
    std::unordered_map<BundleHandle, BundleManifest> m_Manifests;
    std::unique_ptr<Asset::IBundleProcessor> m_Processor;
    std::unique_ptr<Asset::IBundleCache> m_Cache;
    std::unique_ptr<Asset::IPatchManager> m_PatchManager;
    std::unique_ptr<Asset::IManifestManager> m_ManifestManager;
    std::unique_ptr<Asset::IBundleValidator> m_Validator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 