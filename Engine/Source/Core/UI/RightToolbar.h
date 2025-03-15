/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RightToolbar.h
 * @brief The right toolbar is a specialized view for displaying tools and options.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
/** 
 * @brief Displays the right toolbar in the UI.
 * @param p_open A pointer to a boolean variable. If not NULL, the right toolbar 
 *               will display a close button in the upper-right corner, and 
 *               clicking it will set the variable to false.
 */
void ShowRightToolbar(bool* p_open, EditorWindowData* windowData);

} // namespace hd