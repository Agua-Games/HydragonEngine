/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SettingsEditor.h
 * @brief The Settings Editor is a specialized view for managing settings.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

void showSettingsEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd