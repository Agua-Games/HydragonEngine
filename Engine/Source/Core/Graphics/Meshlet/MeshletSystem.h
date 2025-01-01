/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#pragma once
#include "Core/Graphics/RenderAPI/IRenderDevice.h"
#include "Core/Memory/Management/Strategies/GraphicsMemoryStrategy.h"

namespace Hydragon {
namespace Graphics {

// API-agnostic meshlet configuration
struct MeshletConfig {
    // Common limits that work for both DX12 and Vulkan
    static constexpr uint32_t MAX_VERTICES = 64;      // Works for both APIs
    static constexpr uint32_t MAX_PRIMITIVES = 126;   // Common limit
    static constexpr uint32_t WORKGROUP_SIZE = 32;    // Configurable per API
};

// Abstract base for API-specific implementations
class IMeshletProcessor {
public:
    virtual void buildMeshlets(const MeshData& data) = 0;
    virtual void dispatchMeshTasks(const ViewContext& view) = 0;
    virtual ~IMeshletProcessor() = default;
};

// API-specific implementations
class DX12MeshletProcessor : public IMeshletProcessor {
    void buildMeshlets(const MeshData& data) override {
        // DirectX 12 specific implementation
        if (m_Device->supportsMeshShaders()) {
            buildDX12Meshlets(data);
        }
    }
};

class VulkanMeshletProcessor : public IMeshletProcessor {
    void buildMeshlets(const MeshData& data) override {
        // Vulkan specific implementation
        if (m_Device->supportsTaskShaders()) {
            buildVulkanMeshlets(data);
        }
    }
};

// Factory for creating API-specific processors
class MeshletProcessorFactory {
public:
    static std::unique_ptr<IMeshletProcessor> create(RenderAPI api) {
        switch (api) {
            case RenderAPI::DirectX12:
                return std::make_unique<DX12MeshletProcessor>();
            case RenderAPI::Vulkan:
                return std::make_unique<VulkanMeshletProcessor>();
            default:
                return nullptr;
        }
    }
};

}} // namespace Hydragon::Graphics 