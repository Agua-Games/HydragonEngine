/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Streaming outliner for Hydragon
 */

#include "StreamingOutliner.h"
#include <QHeaderView>

namespace Hydragon {
namespace Tools {

StreamingOutliner::StreamingOutliner(QWidget* parent) : QTreeWidget(parent) {
    setupTreeStructure();
    
    // Connect signals
    connect(this, &QTreeWidget::itemExpanded, 
            this, &StreamingOutliner::onItemExpanded);
}

void StreamingOutliner::setupTreeStructure() {
    setColumnCount(4);
    setHeaderLabels({
        "Streamer",
        "Active/Pending",
        "Memory Used",
        "Status"
    });
    
    // Setup categories
    auto textureStreamer = createCategory("Texture Streaming");
    auto meshStreamer = createCategory("Mesh Streaming");
    auto audioStreamer = createCategory("Audio Streaming");
    
    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setStretchLastSection(true);
}

void StreamingOutliner::addStreamer(const StreamerNode& streamer) {
    auto item = new StreamerTreeItem(this);
    item->streamerName = streamer.name;
    item->setText(0, streamer.name);
    item->setText(1, QString("%1/%2").arg(streamer.activeStreams)
                                    .arg(streamer.pendingRequests));
    item->setText(2, QString::number(streamer.memoryUsed / 1024.0f, 'f', 2) + " MB");
    
    // Add status indicator
    auto statusWidget = new StreamerStatusWidget(streamer.type);
    setItemWidget(item, 3, statusWidget);
    
    m_StreamerItems[streamer.name] = item;
}

void StreamingOutliner::updateStreamerStats(const QString& name, 
                                          const StreamerStats& stats) {
    if (auto it = m_StreamerItems.find(name); it != m_StreamerItems.end()) {
        auto item = it->second;
        
        // Update stats
        item->setText(1, QString("%1/%2").arg(stats.activeStreams)
                                        .arg(stats.pendingRequests));
        item->setText(2, QString::number(stats.memoryUsed / 1024.0f, 'f', 2) + " MB");
        
        // Update resource list if expanded
        if (item->isExpanded()) {
            updateResourceList(item, stats.resources);
        }
        
        // Update status
        if (auto statusWidget = qobject_cast<StreamerStatusWidget*>(
                itemWidget(item, 3))) {
            statusWidget->updateStatus(stats);
        }
    }
}

void StreamingOutliner::updateResourceList(StreamerTreeItem* item,
                                         const std::vector<StreamingResource>& resources) {
    // Clear old resources
    for (auto* resourceItem : item->resourceItems) {
        delete resourceItem;
    }
    item->resourceItems.clear();
    
    // Add new resources
    for (const auto& resource : resources) {
        auto resourceItem = new QTreeWidgetItem(item);
        resourceItem->setText(0, resource.name);
        resourceItem->setText(1, QString::number(resource.loadProgress, 'f', 1) + "%");
        resourceItem->setText(2, QString::number(resource.size / 1024.0f, 'f', 2) + " MB");
        
        item->resourceItems.push_back(resourceItem);
    }
}

}} // namespace Hydragon::Tools 