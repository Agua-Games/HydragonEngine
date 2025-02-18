/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <functional>
#include <string>
#include <unordered_map>

namespace hdImgui {

struct NodeGraphState {
    float zoomLevel = 1.0f;
    ImVec2 panOffset = ImVec2(0.0f, 0.0f);
    bool isDragging = false;
    bool isConnecting = false;
    int selectedNode = -1;
    ImVec2 lastMousePos;
    bool showMiniMap = true;
    bool showNodeLibrary = true;
    float nodeLibraryWidth = 250.0f;
    bool showRightSidebar = true;
    int nodeCount = 0;
    int connectionCount = 0;
    float graphMemoryUsage = 0.0f;    // MB
    int warningCount = 0;
    int errorCount = 0;
    float fps = 0.0f;
    bool isGraphDirty = false;

    // Node registration
    using NodeCreatorFunc = std::function<void()>;
    std::unordered_map<std::string, NodeCreatorFunc> nodeTypes;

    void RegisterNodeType(const std::string& type, NodeCreatorFunc creator) {
        nodeTypes[type] = creator;
    }

    // Callbacks
    std::function<bool(int, int)> onNodeConnect;
    std::function<void(int)> onNodeSelected;
};

} // namespace hdImgui