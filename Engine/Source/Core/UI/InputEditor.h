/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InputEditor.h
 * @brief The Input Editor is a specialized view for input-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowInputEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui