#include "MemoryProfiler.h"
#include "Exporters/RenderDocExporter.h"
#include <thread>

namespace Hydragon {
namespace Memory {

void MemoryProfiler::BeginCapture(const char* tag) {
    m_CurrentCapture = std::make_unique<CaptureSession>();
    m_CurrentCapture->tag = tag;
    m_CurrentCapture->startTime = std::chrono::steady_clock::now();
    
    // Start RenderDoc capture if enabled
    if (m_RenderDocExporter) {
        m_RenderDocExporter->BeginCapture();
    }
    
    // Begin tracking allocations
    m_IsCapturing = true;
}

void MemoryProfiler::TrackAllocation(void* ptr, size_t size) {
    if (!m_IsCapturing) return;
    
    AllocationEvent event;
    event.ptr = ptr;
    event.size = size;
    event.timestamp = std::chrono::steady_clock::now();
    event.threadId = std::this_thread::get_id();
    
    m_CurrentCapture->events.push_back(event);
}

}} // namespace Hydragon::Memory 