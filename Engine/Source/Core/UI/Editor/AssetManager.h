/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AssetEditor.h
 * @brief The Asset Editor is a specialized view for asset-based nodes.
 */
#pragma once
#include <imgui.h>  // Include ImGui's header

#include "UIManager.h"

namespace hd {

/**
 * @brief Renders the main asset editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showAssetEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd