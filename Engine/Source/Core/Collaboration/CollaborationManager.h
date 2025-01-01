/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collaboration and real-time data sharing
 */

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