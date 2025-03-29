/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RenderTypes.h
 * @brief Header file for the RenderTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - RenderTypes is a namespace that contains various types and enums related to rendering.
 * - It includes types for render targets, render passes, and more.
 * 
 * @todo Move the many render types from PrismaRenderer.h to here.
 */
#pragma once

namespace hd {

// Define render target types
enum class RenderTargetType {
    Color,
    Depth,
    Normal,
    Custom
};

// Define render pass types
enum class RenderPassType {
    Forward,
    Deferred,
    Custom
};

enum AOQuality {
    Low,
    Medium,
    High
};

} // namespace hd

