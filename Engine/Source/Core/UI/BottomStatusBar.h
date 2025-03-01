/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BottomStatusBar.h
 * @brief The bottom status bar is a specialized view for displaying status information.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowBottomStatusBar(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui