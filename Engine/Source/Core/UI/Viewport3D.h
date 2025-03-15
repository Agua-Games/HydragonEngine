/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Viewport3D.h
 * @brief The Viewport 3D is a specialized view for 3D content.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

void ShowViewport3D(bool* p_open, EditorWindowData* windowData);

} // namespace hd