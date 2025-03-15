/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BottomStatusBar.h
 * @brief The bottom status bar is a specialized view for displaying status information.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

/** 
 * @brief Displays the bottom status bar in the UI.
 * @param p_open A pointer to a boolean variable. If not NULL, the bottom status bar 
 *               will display a close button in the upper-right corner, and 
 *               clicking it will set the variable to false.
 */
void ShowBottomStatusBar(bool* p_open, EditorWindowData* windowData);

} // namespace hd