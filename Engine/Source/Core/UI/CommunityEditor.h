/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CommunityEditor.h
 * @brief The Community Editor is a specialized view for community-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowCommunityEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui