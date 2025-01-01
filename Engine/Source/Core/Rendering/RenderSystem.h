#pragma once
#include "Core/Memory/Management/Strategies/GraphicsMemoryStrategy.h"

namespace Hydragon {
namespace Rendering {

class RenderSystem {
public:
    // Support for both editor viewport and final output
    struct ViewportDesc {
        uint32_t width;
        uint32_t height;
        bool enableRealTimeUpdate = true;
        float refreshRate = 60.0f;
    };

    // Integration with node graph system
    void executeRenderGraph(const RenderGraph& graph) {
        Memory::ScopedAllocation alloc(m_MemoryStrategy);
        // Execute render passes
    }
};

/*
    Python usage:
    viewport = hd.render.viewport()
    
    with hd.render.graph() as g:
        g.add_pass("geometry")
        g.add_pass("lighting")
        viewport.show(g)
*/

}} // namespace Hydragon::Rendering 