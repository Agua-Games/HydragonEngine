/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CollaborationEditor.h
 * @brief The Collaboration Editor is a specialized view for collaboration-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showCollaborationEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd