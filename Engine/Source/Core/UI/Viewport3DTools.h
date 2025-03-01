/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Viewport3DTools.h
 * @brief The Viewport 3D Tools is a specialized viewport overlay window for managing 3D content.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {

void ShowViewport3DTools(bool* p_open, HdEditorWindowData* windowData);

} // namespace hdImgui