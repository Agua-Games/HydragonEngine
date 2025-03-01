/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LightingEditor.h
 * @brief The Lighting Editor is a specialized view for lighting-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowLightingEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui