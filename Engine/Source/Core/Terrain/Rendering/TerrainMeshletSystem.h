/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#pragma once
#include "Core/Graphics/Meshlet/MeshletSystem.h"
#include "Core/Graphics/RenderAPI/IRenderDevice.h"

namespace Hydragon {
namespace Terrain {

class TerrainMeshletSystem {
public:
    TerrainMeshletSystem(Graphics::RenderAPI api) 
        : m_MeshletProcessor(Graphics::MeshletProcessorFactory::create(api)) 
    {
        initializeForAPI(api);
    }

    void generateMeshlets(const TerrainChunk& chunk) {
        // API-agnostic mesh data preparation
        Graphics::MeshData meshData = prepareMeshData(chunk);
        
        // Let the API-specific processor handle it
        m_MeshletProcessor->buildMeshlets(meshData);
    }

private:
    void initializeForAPI(Graphics::RenderAPI api) {
        switch (api) {
            case Graphics::RenderAPI::DirectX12:
                m_ShaderCompiler = std::make_unique<DX12ShaderCompiler>();
                break;
            case Graphics::RenderAPI::Vulkan:
                m_ShaderCompiler = std::make_unique<VulkanShaderCompiler>();
                break;
        }
    }

    std::unique_ptr<Graphics::IMeshletProcessor> m_MeshletProcessor;
    std::unique_ptr<IShaderCompiler> m_ShaderCompiler;
};

// Python API remains the same, abstraction is hidden
/*
    import hydragon as hd
    
    # API selection is handled internally
    terrain = hd.terrain.create()
    
    # Same interface regardless of API
    terrain.meshlets.configure(
        max_meshlets=32768,
        cull_size=0.5
    )
*/

}} // namespace Hydragon::Terrain 