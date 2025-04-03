/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ImageTools.h
 * @brief The Image Tools is a specialized viewport overlay window for managing images.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

/** 
 * @brief Displays the Image Tools window in the UI.
 * @param p_open A pointer to a boolean variable. If not NULL, the Image Tools 
 *               will display a close button in the upper-right corner, and 
 *               clicking it will set the variable to false.
 */
void showImageTools(bool* p_open, EditorWindowData* windowData);

} // namespace hd