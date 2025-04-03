/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FontEditor.h
 * @brief The Font Editor is a specialized view for managing fonts.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

/**
 * @brief Renders the main font editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showFontEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd