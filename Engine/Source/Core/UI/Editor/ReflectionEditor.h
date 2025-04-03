/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ReflectionEditor.h
 * @brief The Reflection Editor is a specialized view for reflection-based nodes.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

void showReflectionEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd