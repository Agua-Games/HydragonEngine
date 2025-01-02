/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Filter preset system for Hydragon
 */

#pragma once
#include "MemoryFilterSystem.h"
#include <QSettings>

namespace Hydragon {
namespace Tools {

class FilterPresetSystem {
public:
    struct FilterPreset {
        QString name;
        QString description;
        MemoryFilterSystem::FilterCriteria criteria;
        bool isBuiltIn;
        
        // Metadata
        QDateTime created;
        QDateTime lastUsed;
        int useCount;
    };

    // Preset management
    void savePreset(const QString& name, const FilterCriteria& criteria);
    void loadPreset(const QString& name);
    void deletePreset(const QString& name);
    
    // Built-in presets
    void initializeDefaultPresets();
    
    // User preferences
    void setAutoSaveEnabled(bool enable);
    void setMaxPresets(size_t count);

private:
    std::unordered_map<QString, FilterPreset> m_Presets;
    QSettings m_Settings;
    
    // Built-in presets
    void addMemoryPressurePresets();
    void addPerformancePresets();
    void addDebugPresets();
};

}} // namespace Hydragon::Tools 