/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FileExplorer.h
 * @brief The File Explorer is a specialized view for managing files and directories.
 */
#pragma once
#include <imgui.h>
#include <string>

#include "hdImgui.h"

namespace hdImgui {
void ShowFileExplorer(bool* p_open, HdEditorWindowData* windowData);
} // namespace hdImgui
