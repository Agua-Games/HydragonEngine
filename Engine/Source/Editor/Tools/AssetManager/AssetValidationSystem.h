/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset validation and verification system
 */

#pragma once
#include "AssetTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct AssetValidationConfig {
    bool enableAutoValidation = true;
    bool enableReferenceChecking = true;
    bool enableFormatValidation = true;
    bool enableSizeChecks = true;
    uint32_t maxValidationThreads = 4;
    size_t maxAssetSize = 2ULL * 1024 * 1024 * 1024;  // 2GB
};

class AssetValidationSystem {
public:
    static AssetValidationSystem& Get();
    
    void Initialize(const AssetValidationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void ValidateAsset(const std::string& path);
    void ValidateFolder(const std::string& path);
    void ValidateReferences(const std::string& path);
    
    void AddValidationRule(const std::string& assetType, 
                          const ValidationRule& rule);
    void RemoveValidationRule(const std::string& assetType, 
                            const std::string& ruleName);
    
    void RegisterValidator(const std::string& assetType, 
                         std::unique_ptr<IAssetValidator> validator);
    
    ValidationResult GetValidationResult(const std::string& path) const;
    bool IsValidating(const std::string& path) const;
    
    const AssetValidationStats& GetStats() const { return m_Stats; }

private:
    AssetValidationSystem() = default;
    
    AssetValidationConfig m_Config;
    AssetValidationStats m_Stats;
    std::vector<ValidationTask> m_Tasks;
    std::unique_ptr<IValidationManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 