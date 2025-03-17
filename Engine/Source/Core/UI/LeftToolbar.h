/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LeftToolbar.h
 * @brief The left toolbar is a specialized view for displaying tools and options.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showLeftToolbar(bool* p_open, EditorWindowData* windowData);
} // namespace hd