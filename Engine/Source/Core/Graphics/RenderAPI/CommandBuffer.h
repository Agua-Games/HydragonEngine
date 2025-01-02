/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Command buffer for render commands
 */

#pragma once
#include "../RenderTypes.h"
#include <vector>

namespace Hydragon::Graphics {

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;
    
    virtual void BindPipeline(PipelineHandle pipeline) = 0;
    virtual void BindVertexBuffer(BufferHandle buffer, uint32_t binding) = 0;
    virtual void BindIndexBuffer(BufferHandle buffer) = 0;
    
    virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) = 0;
    virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;
    
    virtual void SetViewport(float x, float y, float width, float height) = 0;
    virtual void SetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;

protected:
    std::vector<CommandBuffer*> m_SecondaryBuffers;
    bool m_IsRecording = false;
};

} // namespace Hydragon::Graphics 