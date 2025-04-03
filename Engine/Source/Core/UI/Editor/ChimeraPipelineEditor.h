/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ChimeraPipelineEditor.h
 * @brief The Chimera Pipeline Editor is a specialized view for pipeline-based nodes.
 */
#pragma once
#include <imgui.h>

#include "MainEditorUIManager.h"

namespace hd {

/**
 * @brief Renders the Chimera Pipeline Editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showChimeraPipelineEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd