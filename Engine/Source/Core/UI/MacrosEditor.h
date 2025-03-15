/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MacrosEditor.h
 * @brief The Macros Editor is a specialized view for macro-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowMacrosEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd