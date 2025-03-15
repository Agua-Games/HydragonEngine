/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DramaEditor.h
 * @brief The Drama Editor is a specialized view for drama-based nodes.
 * 
 * ARCHITECTURAL NOTES:
 * The Drama Editor provides a visual interface for the engine's drama-based
 * architecture, implementing the following key features:
 * - Setup of dramatic sequences and events using node graphs
 * - Integration of drama elements with narrative and character arcs
 * - Visualization of drama elements and their relationships
 * - Delegation of property editing to the central Properties window
 * - Provision of quick access to common drama operations
 * - It must be fleshed out to comply with the engine's 3-way architecture: nodal, scripting and natural language
 */
#pragma once
#include <imgui.h>
#include "UIManager.h"

namespace hd {

/**
 * @brief Renders the main drama editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void ShowDramaEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd
