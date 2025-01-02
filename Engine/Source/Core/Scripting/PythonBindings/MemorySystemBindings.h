/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Python bindings for memory system functionality
 */

#pragma once
#include <pybind11/pybind11.h>
#include "Core/Memory/MemorySystem.h"

namespace Hydragon::Scripting {

/**
 * Provides Python bindings for memory system functionality
 */
class MemorySystemBindings {
private:
    /**
     * Registers memory-related bindings with Python
     * @param m Python module to bind to
     */
    void registerMemoryBindings(pybind11::module& m) {
        namespace py = pybind11;
        
        // Use Pythonic naming (PEP 8)
        py::class_<Memory::MemorySystem>(m, "memory")
            .def("get_usage", &Memory::MemorySystem::getUsage)
            .def("clear_cache", &Memory::MemorySystem::clearCache)
            .def("watch_allocation", &Memory::MemorySystem::trackAllocation)
            .def("track_node", [](const char* node_name) {
                return Memory::MemorySystem::get().beginNodeMemoryTracking(node_name);
            });
    }
};

// Example of more Pythonic usage:
/*
    import hydragon as hd
    
    # More natural Python style
    mem = hd.memory()
    
    # Simple, clear methods
    mem.watch_allocation("my_node")
    
    print(f"Memory used: {mem.total_used}")
    
    # Feels more like standard Python
    with mem.track_node("process_node"):
        # Node processing
        pass
*/

} // namespace Hydragon::Scripting 