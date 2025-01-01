/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset system for Hydragon
 */

#pragma once
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"

namespace Hydragon {
namespace Assets {

class AssetSystem {
public:
    // Support for both editor and runtime asset management
    struct AssetLoadConfig {
        bool enableVirtualization = true;
        bool allowPartialLoading = true;
        size_t streamingBudget = 512 * 1024 * 1024;  // 512MB
    };

    // Python-friendly asset operations
    AssetHandle loadAsset(const char* path, const LoadParams& params = {}) {
        return internalLoadAsset(path, params);
    }
};

// Python bindings would look like:
/*
    import hydragon as hd
    
    # Simple asset loading
    model = hd.assets.load("models/character.fbx")
    
    # With streaming options
    texture = hd.assets.load("textures/large.dds", 
                           streaming=True, 
                           priority="high")
*/

}} // namespace Hydragon::Assets 