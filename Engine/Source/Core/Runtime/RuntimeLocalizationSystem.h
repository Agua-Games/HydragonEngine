/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime localization and language management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeLocalizationConfig {
    bool enableDynamicLoading = true;
    bool enableFallback = true;
    bool enablePluralization = true;
    bool enableContextualVariants = true;
    uint32_t maxConcurrentLoads = 4;
    uint32_t cacheSize = 1024;
    std::string defaultLocale = "en-US";
    std::string fallbackLocale = "en-US";
};

class RuntimeLocalizationSystem {
public:
    static RuntimeLocalizationSystem& Get();
    
    void Initialize(const RuntimeLocalizationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void LoadLocale(const std::string& locale, bool setActive = false);
    void UnloadLocale(const std::string& locale);
    
    void SetActiveLocale(const std::string& locale);
    std::string GetActiveLocale() const;
    
    std::string GetText(const std::string& key, const LocalizationParams& params = {});
    std::string GetPlural(const std::string& key, int count, const LocalizationParams& params = {});
    std::string GetContextual(const std::string& key, const std::string& context, const LocalizationParams& params = {});
    
    void RegisterFormatter(const std::string& name, std::unique_ptr<ILocalizationFormatter> formatter);
    void UnregisterFormatter(const std::string& name);
    
    bool HasLocale(const std::string& locale) const;
    bool HasKey(const std::string& key) const;
    
    const RuntimeLocalizationStats& GetStats() const { return m_Stats; }

private:
    RuntimeLocalizationSystem() = default;
    
    RuntimeLocalizationConfig m_Config;
    RuntimeLocalizationStats m_Stats;
    std::unordered_map<std::string, LocaleInstance> m_Locales;
    std::unordered_map<std::string, std::unique_ptr<ILocalizationFormatter>> m_Formatters;
    std::unique_ptr<ILocalizationProvider> m_Provider;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 