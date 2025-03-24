/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BottomToolbar.h
 * @brief The bottom toolbar is a specialized view for displaying status information.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showBottomToolbar(bool* p_open, EditorWindowData* windowData);
} // namespace hd