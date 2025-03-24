/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeGraphEditor.h
 * @brief A specialized view for node-based graph editing.
 * 
 * ARCHITECTURAL NOTES:
 * The Node Graph Editor provides a visual interface for the engine's node-graph
 * architecture, implementing the following key features:
 * - All functionality is represented as nodes in the graph
 * - The UI is a visualization layer for the underlying node structure
 * - Sub-editors are consolidated views of related nodes and their properties
 * - It must be fleshed out to comply with the engine's 3-way architecture: nodal, scripting and natural language
 * - The Properties window is the central point for editing node parameters
 * - It uses imgui and the imgui-node-editor (extension) library.
 * - The implementation will be based on the example provided in the imgui-node-editor repository.
 *
 * TODO:
 *  - Cleanup and refactor the whole .cpp file, after each session of bringing code snippets from the imgui-node-editor examples.
 *      - Get rid of unused structs, variables etc.
 *      - Ensure enough basic functionality for the expected engine design (compare often to the most complete example, adapted to the engine's needs)
 *      - Organize the code into logical sections and functions.
 *      - Put the file into proper order, well-structured, cohesive.
 *      - As we beta-test the engine, gradually move it closer to the blueprints-example.cpp file, as this file has the most complete implementation.
 *      - Cleanup temporary comments and unstructured code (e.g. hardcoded values, quick-fix functions, snippets).
 *      - Only after the overall design is consolidated, expose all API-relevant functions here in the header file.
 */
#pragma once
#include <string>                           // Required for imgui-node-editor
#include <imgui.h>                          // Required for imgui-node-editor
#include "imgui_node_editor.h"
#include "Imgui_SteppedLineMath.h"          // Required for stepped line rendering
namespace nodeEd = ax::NodeEditor;

namespace hd {

struct EditorWindowData;

/**
 * @brief Ensure the node editor context is set.
 */
static bool ensureNodeEditorContext();

/**
 * @brief Initialize the node graph editor.
 * Should be called once during application startup.
 * @param windowData Editor window data containing global settings
 */
void initializeNodeGraphEditor(EditorWindowData* windowData);

/**
 * @brief Shutdown the node graph editor and releases resources.
 */
void shutdownNodeGraphEditor();

/**
 * @brief Renders the main node graph editor window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void showNodeGraphEditor(bool* p_open, EditorWindowData* windowData);

/**
 * @brief Renders the graph canvas area.
 * @param windowData Editor window data containing global settings
 */
void renderGraphCanvas(EditorWindowData* windowData);

/**
 * @brief Renders the minimap overlay.
 */
void renderMiniMap();

/**
 * @brief Creates a node with a styled title bar.
 * @param nodeId Unique identifier for the node
 * @param title Text to display in the title bar
 * @param titleBarColor Color of the title bar
 * @param nodeColor Color of the node body
 */
void beginNodeWithTitleBar(nodeEd::NodeId nodeId, const char* title, 
    ImColor titleBarColor, ImColor nodeColor);

} // namespace hd
