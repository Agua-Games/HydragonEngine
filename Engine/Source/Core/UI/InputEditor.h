/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InputEditor.h
 * @brief The Input Editor is a specialized view for input-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

/**
 * @brief Renders the main input editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void ShowInputEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd