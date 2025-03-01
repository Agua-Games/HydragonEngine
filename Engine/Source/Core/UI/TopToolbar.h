/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TopToolbar.h
 * @brief The top toolbar is a specialized view for displaying tools and options.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {

void ShowTopToolbar(bool* p_open, HdEditorWindowData* windowData);

} // namespace hdImgui