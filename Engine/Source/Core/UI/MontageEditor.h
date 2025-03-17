/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MontageEditor.h
 * @brief The Montage Editor is a specialized view for montage-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void showMontageEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd