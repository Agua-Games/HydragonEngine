#include "MemoryHeatmapView.h"
#include <QPainter>
#include <QGradient>

namespace Hydragon {
namespace Tools {

void MemoryHeatmapView::updateHeatmap(const MemoryState& state) {
    // Update cell intensities
    for (const auto& block : state.blocks) {
        QPointF pos = mapAddressToPosition(block.address);
        float intensity = calculateBlockIntensity(block);
        updateCellIntensity(pos.x(), pos.y(), intensity);
    }
    
    // Apply temporal decay
    applyIntensityDecay();
    
    // Update visuals
    updateCellColors();
}

void MemoryHeatmapView::updateCellColors() {
    const QColor coldColor(0, 0, 255);    // Blue
    const QColor hotColor(255, 0, 0);     // Red
    
    for (auto& row : m_Cells) {
        for (auto& cell : row) {
            float t = cell.intensity;
            QColor color = QColor::fromRgbF(
                hotColor.redF() * t + coldColor.redF() * (1-t),
                hotColor.greenF() * t + coldColor.greenF() * (1-t),
                hotColor.blueF() * t + coldColor.blueF() * (1-t)
            );
            
            cell.rect->setBrush(color);
            
            // Add glow effect for high intensity
            if (t > 0.8f) {
                auto effect = new QGraphicsDropShadowEffect;
                effect->setColor(color);
                effect->setBlurRadius(10 * t);
                cell.rect->setGraphicsEffect(effect);
            }
        }
    }
}

QPointF MemoryHeatmapView::mapAddressToPosition(void* addr) {
    // Map memory address to grid position
    uintptr_t address = reinterpret_cast<uintptr_t>(addr);
    int gridSize = m_Config.gridSize;
    
    // Use memory layout information to map addresses to 2D space
    int x = (address / m_Config.pageSize) % gridSize;
    int y = ((address / m_Config.pageSize) / gridSize) % gridSize;
    
    return QPointF(x, y);
}

}} // namespace Hydragon::Tools 