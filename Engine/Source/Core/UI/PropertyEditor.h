/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PropertyEditor.h
 * @brief The Property Editor is a specialized view for property-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
/**
 * @brief Renders the main property editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void ShowPropertyEditor(bool* p_open, HdEditorWindowData* windowData);

} // namespace hdImgui