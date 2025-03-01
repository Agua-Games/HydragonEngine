/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AgentsEditor.h
 * @brief The Agents Editor is a specialized view for agent-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowAgentsEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui