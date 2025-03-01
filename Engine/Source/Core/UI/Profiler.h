/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Profiler.h
 * @brief The Profiler is a specialized view for performance analysis.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowProfiler(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui