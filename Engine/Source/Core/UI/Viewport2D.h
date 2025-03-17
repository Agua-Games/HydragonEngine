/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Viewport2D.h
 * @brief The Viewport 2D is a specialized view for 2D content.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

void showViewport2D(bool* p_open, EditorWindowData* windowData);

} // namespace hd