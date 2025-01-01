#pragma once
#include "../MemoryProfiler.h"
#include <renderdoc_app.h>

namespace Hydragon {
namespace Memory {

class RenderDocExporter {
public:
    void BeginCapture();
    void EndCapture();
    
    void ExportMemoryEvents(const std::vector<MemoryEvent>& events);
    void AnnotateAllocation(void* ptr, size_t size, const char* tag);

private:
    RENDERDOC_API_1_1_2* m_RenderDoc = nullptr;
};

}} // namespace Hydragon::Memory 