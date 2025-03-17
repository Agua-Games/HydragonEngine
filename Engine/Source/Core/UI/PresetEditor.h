/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PresetEditor.h
 * @brief The Preset Editor is a specialized view for preset-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showPresetEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd