#pragma once
#include <QTreeWidget>
#include "Core/Memory/Streaming/StreamerTypes.h"

namespace Hydragon {
namespace Tools {

class StreamingOutliner : public QTreeWidget {
    Q_OBJECT
public:
    explicit StreamingOutliner(QWidget* parent = nullptr);
    
    struct StreamerNode {
        QString name;
        StreamerType type;
        size_t activeStreams;
        size_t pendingRequests;
        float memoryUsed;
    };
    
    void addStreamer(const StreamerNode& streamer);
    void updateStreamerStats(const QString& name, const StreamerStats& stats);
    
private:
    struct StreamerTreeItem : QTreeWidgetItem {
        QString streamerName;
        std::vector<QTreeWidgetItem*> resourceItems;
    };
    
    std::map<QString, StreamerTreeItem*> m_StreamerItems;
    
    void setupTreeStructure();
    void updateResourceList(StreamerTreeItem* item, 
                          const std::vector<StreamingResource>& resources);
};

}} // namespace Hydragon::Tools 