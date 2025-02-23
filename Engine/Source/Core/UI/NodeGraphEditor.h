/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include "imgui.h"

namespace hdImgui {

struct HdEditorWindowData;

/**
 * Shows the node graph editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void ShowNodeGraphEditor(bool* p_open, HdEditorWindowData* windowData);

/**
 * Renders the graph canvas area.
 * @param windowData Editor window data containing global settings
 */
void RenderGraphCanvas(HdEditorWindowData* windowData);

/**
 * Renders the minimap overlay.
 */
void RenderMiniMap();

} // namespace hdImgui
