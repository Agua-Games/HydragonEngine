/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ImageTools.h
 * @brief The Image Tools is a specialized viewport overlay window for managing images.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowImageTools(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui