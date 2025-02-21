
/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * Vulkan backend for the engine.
 * Wraps Vulkan handles and provides access to them.
 * 
 */
#pragma once
//#include "VulkanBackend.h"        // Uncomment after finishing migration to new backend. It was commented out because it's already included in VulkanBackend_Legacy.h
#include "VulkanBackend_Legacy.h"   // Only remove this line after finishing migration to new backend.

namespace hd {
 
VulkanBackend::~VulkanBackend() {
    if (initialized) {
        try {
            Shutdown();
        } catch (const std::exception& e) {
            std::cerr << "Error during shutdown: " << e.what() << std::endl;
        }
    }
}

} // namespace hd
