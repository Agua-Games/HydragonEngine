/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor state persistence and session management
 */

#pragma once

namespace Hydragon {
namespace Editor {

class EditorStatePersistence {
    // Preserves editor state across sessions
    struct EditorState {
        std::vector<WindowLayout> layouts;
        std::map<std::string, NodeGraphState> graphStates;
        RecentFilesList recentFiles;
    };
};

}} // namespace Hydragon::Editor 