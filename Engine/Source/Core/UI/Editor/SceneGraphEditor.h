/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SceneGraphEditor.h
 * @brief The Scene Graph Editor is a specialized view for scene-based nodes.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {

void showSceneGraphEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd