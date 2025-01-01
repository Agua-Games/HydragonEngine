#include "InteractiveMemoryView.h"
#include <QMouseEvent>
#include <QMenu>

namespace Hydragon {
namespace Tools {

InteractiveMemoryView::InteractiveMemoryView(QWidget* parent) 
    : QGraphicsView(parent)
    , m_Scene(new QGraphicsScene(this))
    , m_ComparisonMode(false) {
    
    setScene(m_Scene);
    setupInteractions();
}

void InteractiveMemoryView::setupInteractions() {
    // Enable drag mode for panning
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    
    // Setup context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &InteractiveMemoryView::showContextMenu);
            
    // Timeline slider
    m_TimelineSlider = new QSlider(Qt::Horizontal, this);
    connect(m_TimelineSlider, &QSlider::valueChanged,
            this, &InteractiveMemoryView::onTimelineChanged);
            
    // Search box
    m_SearchBox = new QLineEdit(this);
    m_SearchBox->setPlaceholderText("Search by address or tag...");
    connect(m_SearchBox, &QLineEdit::textChanged,
            this, &InteractiveMemoryView::onSearchTextChanged);
}

void InteractiveMemoryView::showContextMenu(const QPoint& pos) {
    QMenu menu(this);
    
    // Block operations
    if (auto block = getBlockAtPosition(pos)) {
        menu.addAction("Group Similar", [this, block]() {
            groupSimilarBlocks(block);
        });
        
        if (m_ComparisonMode) {
            menu.addAction("Compare With Selected", [this, block]() {
                compareBlocks(m_SelectedBlock, block);
            });
        } else {
            menu.addAction("Start Comparison", [this, block]() {
                m_ComparisonMode = true;
                m_SelectedBlock = block;
            });
        }
        
        menu.addSeparator();
    }
    
    // View options
    auto filterMenu = menu.addMenu("Filter By");
    filterMenu->addAction("Size > 1MB", [this]() { filterBySize(1024*1024); });
    filterMenu->addAction("Type", [this]() { showTypeFilter(); });
    
    menu.exec(mapToGlobal(pos));
}

void InteractiveMemoryView::compareBlocks(const Memory::MemoryBlock* a, 
                                        const Memory::MemoryBlock* b) {
    if (!a || !b) return;
    
    ComparisonDialog dialog(this);
    dialog.setBlocks(a, b);
    dialog.exec();
    
    m_ComparisonMode = false;
    m_SelectedBlock = nullptr;
}

void InteractiveMemoryView::onTimelineChanged(int value) {
    if (auto snapshot = m_History.getSnapshot(value)) {
        updateView(*snapshot);
        emit timePointSelected(snapshot->timestamp);
    }
}

void InteractiveMemoryView::onSearchTextChanged(const QString& text) {
    // Clear previous highlights
    for (auto& highlight : m_SearchHighlights) {
        highlight->setBrush(m_DefaultBrush);
    }
    m_SearchHighlights.clear();
    
    if (text.isEmpty()) return;
    
    // Search by address or tag
    bool isAddress = text.startsWith("0x");
    for (auto* block : m_BlockTree->getAllBlocks()) {
        bool match = false;
        if (isAddress) {
            match = QString::number((quintptr)block->data, 16).contains(
                text.mid(2), Qt::CaseInsensitive);
        } else {
            match = block->tag.contains(text, Qt::CaseInsensitive);
        }
        
        if (match) {
            highlightBlock(block);
        }
    }
}

}} // namespace Hydragon::Tools 