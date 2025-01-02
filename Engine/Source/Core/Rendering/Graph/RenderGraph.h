/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Render graph system for frame resource management
 */

#pragma once
#include "../RenderTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Rendering {

class RenderGraph {
public:
    struct PassDesc {
        std::string name;
        std::vector<ResourceHandle> inputs;
        std::vector<ResourceHandle> outputs;
        std::function<void(CommandBuffer&)> execute;
    };

    RenderGraph() = default;
    
    ResourceHandle CreateResource(const ResourceDesc& desc);
    PassHandle AddPass(const PassDesc& desc);
    
    void SetInput(PassHandle pass, const std::string& name, ResourceHandle resource);
    void SetOutput(PassHandle pass, const std::string& name, ResourceHandle resource);
    
    void AddDependency(PassHandle dependent, PassHandle dependency);
    void RemoveDependency(PassHandle dependent, PassHandle dependency);
    
    void Compile();
    void Execute(CommandBuffer& cmd);
    
    void Clear();

private:
    struct Pass {
        PassDesc desc;
        std::vector<PassHandle> dependencies;
        std::vector<PassHandle> dependents;
    };

    std::vector<Pass> m_Passes;
    std::vector<ResourceDesc> m_Resources;
    bool m_Compiled = false;
};

} // namespace Hydragon::Rendering 