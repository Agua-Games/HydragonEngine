/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CommandsPalette.h
 * @brief The Commands Palette is a specialized view for displaying and executing commands.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowCommandsPalette(bool* p_open, EditorWindowData* windowData);
} // namespace hd