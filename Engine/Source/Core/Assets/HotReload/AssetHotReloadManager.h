/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset hot reloading and synchronization
 */

#pragma once
#include "Core/Memory/Management/EditorActionMemory.h"

namespace Hydragon {
namespace Assets {

class AssetHotReloadManager {
public:
    struct HotReloadConfig {
        bool enableFileWatcher = true;
        bool autoReloadAssets = true;
        size_t maxPendingReloads = 32;
    };

    // Handles both editor-triggered and file-system-triggered reloads
    void handleAssetChange(AssetId id, ChangeType type) {
        Memory::EditorActionMemory::ActionContext context{
            .type = Memory::EditorActionMemory::ActionType::AssetReload,
            .identifier = getAssetPath(id),
            .preserveState = shouldPreserveState(type)
        };
        
        // Allocate memory for reload operation
        m_ReloadMemory.allocateForAction(context, calculateReloadSize(id));
    }
};

}} // namespace Hydragon::Assets 