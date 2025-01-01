/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Vulkan-specific meshlet processing implementation
 */

#pragma once
#include "MeshletSystem.h"
#include "Core/Graphics/Vulkan/VulkanDevice.h"

namespace Hydragon {
namespace Graphics {

class VulkanMeshletProcessor : public IMeshletProcessor {
public:
    void buildMeshlets(const MeshData& data) override {
        if (!m_Device->supportsTaskShaders()) {
            return buildFallbackMeshlets(data);
        }

        VkAccelerationStructureBuildGeometryInfoKHR buildInfo{};
        setupAccelerationStructure(buildInfo, data);

        // Use Vulkan mesh shading pipeline
        VkPipelineMeshStateCreateInfoEXT meshState{};
        meshState.maxTaskCount = calculateOptimalTaskCount(data);
        
        // Setup task shader dispatch
        const uint32_t workgroupSize = 
            std::min(MeshletConfig::WORKGROUP_SIZE, 
                    m_Device->getTaskShaderProperties().maxTaskWorkGroupSize);
    }

private:
    void buildFallbackMeshlets(const MeshData& data) {
        // Implement traditional mesh processing when mesh shaders unavailable
        m_FallbackBuilder.processGeometry(data);
    }

    VkDevice m_Device;
    std::unique_ptr<VulkanMeshBuilder> m_MeshBuilder;
};

}} // namespace Hydragon::Graphics 