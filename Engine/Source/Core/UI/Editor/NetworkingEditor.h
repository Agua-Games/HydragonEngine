/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NetworkingEditor.h
 * @brief The Network Editor is a specialized view for network-related nodes.
 * 
 * ARCHITECTURAL NOTES:
 * It follows the engine's node-graph centric architecture by:
 * - Focusing on visualization of network node relationships
 * - Delegating property editing to the central Properties window
 * - Providing quick access to common network operations
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"
//#include "Node.h"
//#include "NetworkNode.h"

namespace hd {

/**
 * @brief Shows the network editor window.
 * This editor visualizes network topology and provides
 * quick access to common network operations, while
 * delegating detailed property editing to the Properties window.
 * 
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showNetworkingEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd