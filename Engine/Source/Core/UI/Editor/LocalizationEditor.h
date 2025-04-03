/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LocalizationEditor.h
 * @brief The Localization Editor is a specialized view for localization-based nodes.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {
void showLocalizationEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd