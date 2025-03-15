/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsEditor.h
 * @brief The Physics Editor is a specialized view for physics-based nodes.
 * 
 * ARCHITECTURAL NOTES:
 * The Physics Editor provides a visual interface for the engine's physics-based
 * architecture, implementing the following key features:
 * - Setup of physics simulations and constraints using node graphs
 * - Integration of physics elements with other systems (e.g., particles, AI)
 * - Visualization of physics elements and their relationships
 * - Delegation of property editing to the central Properties window
 * - Provision of quick access to common physics operations
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowPhysicsEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd