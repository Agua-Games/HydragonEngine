/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ExtensionsEditor.h
 * @brief The Extensions Editor is a specialized view for managing extensions.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowExtensionsEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui