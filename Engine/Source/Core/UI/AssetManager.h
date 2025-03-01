/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AssetManager.h
 * @brief The Asset Manager is a specialized view for asset-based nodes.
 */
#pragma once
#include <imgui.h>  // Include ImGui's header

#include "hdImgui.h"

namespace hdImgui {
void ShowAssetManager(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui