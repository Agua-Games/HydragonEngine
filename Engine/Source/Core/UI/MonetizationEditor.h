/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MonetizationEditor.h
 * @brief The Monetization Editor is a specialized view for monetization-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowMonetizationEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui