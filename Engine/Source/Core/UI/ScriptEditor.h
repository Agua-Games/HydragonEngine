/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScriptEditor.h
 * @brief The Script Editor is a specialized view for script-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
/**
 * @brief Renders the main script editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showScriptEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd