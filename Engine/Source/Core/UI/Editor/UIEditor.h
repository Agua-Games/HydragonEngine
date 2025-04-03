/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIEditor.h
 * @brief The UI Editor is a specialized view for user interface design.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

void showUIEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd