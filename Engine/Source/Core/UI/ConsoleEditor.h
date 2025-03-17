/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ConsoleEditor.h
 * @brief The Console Editor is a specialized view for console-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showConsoleEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd