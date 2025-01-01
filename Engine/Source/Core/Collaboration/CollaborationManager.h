#pragma once

namespace Hydragon {
namespace Collaboration {

class CollaborationManager {
    // Real-time node graph sharing
    void shareNodeGraph(const NodeGraph& graph);
    
    // Live script editing
    void broadcastScriptChanges(const ScriptChangeSet& changes);
    
    // Asset synchronization
    void syncAssetChanges(const AssetChangeSet& changes);
};

}} // namespace Hydragon::Collaboration 