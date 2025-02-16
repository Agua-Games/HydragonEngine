/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Entry point for the Engine Core, as opposed to the Editor (code).
 */
#include <windows.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "Graphics/Vulkan/VulkanBackend.h"

//#include "hdImgui.h"

namespace hd{
    class Engine {
    public:
        // Default constructor and destructor
        Engine() = default;
        ~Engine() = default;
    
        // Custom constructor (optional)
        // ..

    private:

    };
}
