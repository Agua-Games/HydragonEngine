/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Viewport2DTools.h
 * @brief The Viewport 2D Tools is a specialized viewport overlay window for managing 2D content.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

void showViewport2DTools(bool* p_open, EditorWindowData* windowData);

} // namespace hd