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