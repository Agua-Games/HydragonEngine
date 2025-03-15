/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MainMenuBar.h
 * @brief The Main Menu Bar is a specialized view for displaying the main menu bar.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowMainMenuBar(bool* p_open, EditorWindowData* windowData);
} // namespace hd