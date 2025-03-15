/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PerformanceScalabilityEditor.h
 * @brief The Performance Scalability Editor is a specialized view for managing performance and scalability settings.
 */
#pragma once
#include <imgui.h>

#include "UIManager.h"

namespace hd {
void ShowPerformanceScalabilityEditor(bool* p_open, EditorWindowData* windowData);
} // namespace hd