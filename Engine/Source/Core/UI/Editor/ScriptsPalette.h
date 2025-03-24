/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScriptsPalette.h
 * @brief The Scripts Palette is a specialized view for displaying and executing scripts.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

void showScriptsPalette(bool* p_open, EditorWindowData* windowData);

} // namespace hd