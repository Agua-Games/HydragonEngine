#include "RenderDocExporter.h"
#include <renderdoc_app.h>

namespace Hydragon {
namespace Memory {

void RenderDocExporter::BeginCapture() {
    if (!m_RenderDoc) {
        if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
            pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
            if (RENDERDOC_GetAPI) {
                RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&m_RenderDoc);
            }
        }
    }
    
    if (m_RenderDoc) {
        m_RenderDoc->StartFrameCapture(nullptr, nullptr);
    }
}

void RenderDocExporter::ExportMemoryEvents(const std::vector<MemoryEvent>& events) {
    if (!m_RenderDoc) return;
    
    for (const auto& event : events) {
        // Create RenderDoc markers for memory events
        std::string markerName = FormatEventMarker(event);
        m_RenderDoc->SetMarker(0, markerName.c_str());
        
        // Add detailed memory info
        if (event.type == MemoryEvent::Type::Allocation) {
            AnnotateAllocation(event.ptr, event.size, event.tag.c_str());
        }
    }
}

std::string RenderDocExporter::FormatEventMarker(const MemoryEvent& event) {
    std::stringstream ss;
    ss << "Memory " << (event.type == MemoryEvent::Type::Allocation ? "Alloc" : "Free")
       << " [" << event.size << " bytes] "
       << "Tag: " << event.tag;
    return ss.str();
}

}} // namespace Hydragon::Memory 