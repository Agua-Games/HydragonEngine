/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PropertiesMatrixEditor.h
 * @brief The Properties Matrix Editor is a specialized view for managing properties in a matrix format.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowPropertiesMatrixEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd