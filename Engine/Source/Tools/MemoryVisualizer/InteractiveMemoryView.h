#pragma once
#include <QGraphicsView>
#include "BlockQuadTree.h"
#include "StreamingVisualizer.h"

namespace Hydragon {
namespace Tools {

class InteractiveMemoryView : public QGraphicsView {
    Q_OBJECT
public:
    explicit InteractiveMemoryView(QWidget* parent = nullptr);
    
    // Interaction modes
    enum class InteractionMode {
        Pan,
        Select,
        Inspect
    };
    
    void setInteractionMode(InteractionMode mode);
    
signals:
    void blockSelected(const Memory::MemoryBlock& block);
    void streamingBlockInspected(const StreamingBlock& block);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    
private:
    InteractionMode m_Mode = InteractionMode::Select;
    QGraphicsScene* m_Scene;
    
    // Visual elements
    std::unique_ptr<BlockQuadTree> m_BlockTree;
    std::unique_ptr<StreamingVisualizer> m_StreamingViz;
    
    void updateTooltip(const QPointF& pos);
    void handleBlockSelection(const QPointF& pos);
};

}} // namespace Hydragon::Tools 