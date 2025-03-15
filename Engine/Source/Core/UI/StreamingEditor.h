/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StreamingEditor.h
 * @brief The Streaming Editor is a specialized view for streaming-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

void ShowStreamingEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd