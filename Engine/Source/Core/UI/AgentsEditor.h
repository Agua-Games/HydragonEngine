/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header

#include "hdImgui.h"

namespace hdImgui {
    void ShowAgentsEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui