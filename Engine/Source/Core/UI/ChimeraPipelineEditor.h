/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ChimeraPipelineEditor.h
 * @brief The Chimera Pipeline Editor is a specialized view for pipeline-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowChimeraPipelineEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui