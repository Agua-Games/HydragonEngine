/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioEditor.h
 * @brief The Audio Editor is a specialized view for audio-based nodes.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {
void ShowAudioEditor(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui