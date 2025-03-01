/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FontEditor.h
 * @brief The Font Editor is a specialized view for managing fonts.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowFontEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui