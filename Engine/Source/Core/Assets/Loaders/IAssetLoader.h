/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset loader interface
 */

#pragma once
#include "../AssetTypes.h"
#include "Core/Common/Result.h"
#include <vector>

namespace Hydragon::Assets {

class IAssetLoader {
public:
    virtual ~IAssetLoader() = default;

    virtual AssetType GetSupportedType() const = 0;
    virtual std::vector<std::string> GetSupportedExtensions() const = 0;
    
    virtual Result<void*> LoadFromFile(const std::string& path, 
                                     const LoadParams& params) = 0;
    
    virtual Result<void*> LoadFromMemory(const void* data, 
                                       size_t size,
                                       const LoadParams& params) = 0;
                                       
    virtual void Unload(void* data) = 0;
    virtual bool SupportsStreaming() const = 0;
};

} // namespace Hydragon::Assets 