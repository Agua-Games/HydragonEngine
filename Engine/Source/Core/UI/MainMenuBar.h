/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MainMenuBar.h
 * @brief The Main Menu Bar is a specialized view for displaying the main menu bar.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowMainMenuBar(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui