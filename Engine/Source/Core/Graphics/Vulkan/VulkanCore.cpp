
/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VulkanCore.cpp
 * @brief Vulkan core implementation (sometimes called "backend") for the engine.
 * Wraps Vulkan handles and provides access to them.
 * 
 * TODO:
 *  1. Gradually bring the code blocks from VulkanCore_Legacy.h to here, wrapping it in our own classes and functions, always compiling, checking for errors.
 *  2. After complete migration of VulkanCore_Legacy.h, and refactored corresponding parts in main.cpp, replace the content of VulkanCore_Legacy.h with the
 *     content of this file inside of the namespace hd, and remove it from here. In other words: migrate the new content to VulkanCore_Legacy.h, keeping only
 *     the #include "VulkanCore_Legacy.h" line here.
 * 3.  Create the file VulkanCore_Modern.h and add the line #include "VulkanCore_Modern.h" here.
 * 4.  Copy-paste parts of VulkanCore_Legacy.h into VulkanCore_Modern.h, refactor using newly created parts. Then, until we have a solid, stable modern
 *     implementation, error-free, this setup (two headers included here which we can switch between) will allow us to switch between the two implementations.
 * 5.  Once we have a solid, stable modern implementation, we can remove VulkanCore_Legacy.h and rename VulkanCore_Modern.h to VulkanCore.h.
 */
#pragma once
//#include "VulkanCore.h"        // Uncomment after finishing migration to new backend. It was commented out because it's already included in VulkanCore_Legacy.h
#include "VulkanCore_Legacy.h"   // Only remove this line after finishing migration to new backend.

namespace hd {
 
VulkanCore::~VulkanCore() {
    if (initialized) {
        try {
            shutdown();
        } catch (const std::exception& e) {
            std::cerr << "Error during shutdown: " << e.what() << std::endl;
        }
    }
}

} // namespace hd
