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

#include "MainEditorUIManager.h"

namespace hd {
void showFileExplorer(bool* p_open, EditorWindowData* windowData);
} // namespace hd
