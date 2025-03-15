/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PluginEditor.h
 * @brief The Plugin Editor is a specialized view for managing plugins.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowPluginEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd