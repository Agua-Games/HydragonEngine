/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GraphicsTypes.h
 * @brief Header file for the GraphicsTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - GraphicsTypes is a namespace that contains various types and enums related to graphics.
 * - It includes types for graphics APIs, graphics formats, and more.
 * 
 */
#pragma once

namespace hd {

// Define graphics APIs
enum class GraphicsAPI {
    OpenGL,
    DirectX,
    Vulkan,
    Metal,
    Custom
};

// Define graphics formats
enum class GraphicsFormat {
    RGBA,
    RGB,
    RG,
    R,
    Custom
};

} // namespace hd 