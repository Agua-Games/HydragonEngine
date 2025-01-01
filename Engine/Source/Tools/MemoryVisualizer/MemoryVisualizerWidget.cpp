/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory visualizer widget for Hydragon
 */

#include "MemoryVisualizerWidget.h"
#include <QtCharts/QScatterSeries>
#include <QVBoxLayout>
#include <QTimer>

namespace Hydragon {
namespace Tools {

MemoryVisualizerWidget::MemoryVisualizerWidget(QWidget* parent)
    : QWidget(parent)
    , m_UpdateTimer(new QTimer(this))
    , m_BlockView(new MemoryBlockView(this))
{
    setupUI();
    setupCharts();
    
    // Real-time updates
    connect(m_UpdateTimer, &QTimer::timeout, this, &MemoryVisualizerWidget::updateMemoryView);
    m_UpdateTimer->start(1000); // 1 second updates
}

void MemoryVisualizerWidget::setupCharts() {
    // Memory Block Map (2D representation)
    auto blockChart = new QtCharts::QChart();
    m_BlockMapView = new QtCharts::QChartView(blockChart);
    
    // Create grid layout for blocks
    m_BlockSeries = new QtCharts::QScatterSeries();
    m_BlockSeries->setMarkerSize(10);
    
    // Custom mapper to convert memory size to visual representation
    m_BlockMapper = new MemoryBlockMapper(this);
    m_BlockMapper->setXRange(0, 100);  // Normalized space
    m_BlockMapper->setYRange(0, 100);
    
    blockChart->addSeries(m_BlockSeries);
    
    // Memory Usage Timeline
    auto usageChart = new QtCharts::QChart();
    m_MemoryUsageChart = new QtCharts::QChartView(usageChart);
    m_UsageSeries = new QtCharts::QLineSeries();
    usageChart->addSeries(m_UsageSeries);
}

void MemoryVisualizerWidget::updateMemoryView() {
    auto snapshot = takeMemorySnapshot();
    updateBlockMap(snapshot);
    updateUsageTimeline(snapshot);
}

void MemoryVisualizerWidget::updateBlockMap(const MemorySnapshot& snapshot) {
    m_BlockSeries->clear();
    
    // Group small allocations if needed
    struct BlockGroup {
        size_t totalSize;
        int count;
        QString label;
    };
    std::map<std::string, BlockGroup> groups;
    
    for (const auto& block : snapshot.allocations) {
        if (block.size < m_MinBlockSize) {
            // Group small blocks
            auto& group = groups[block.tag];
            group.totalSize += block.size;
            group.count++;
            group.label = QString("%1 (%2 blocks)").arg(block.tag.c_str()).arg(group.count);
        } else {
            // Add large block directly
            QPointF pos = m_BlockMapper->mapToPosition(block);
            m_BlockSeries->append(pos);
        }
    }
    
    // Add grouped blocks
    for (const auto& [tag, group] : groups) {
        QPointF pos = m_BlockMapper->mapGroupToPosition(group);
        m_BlockSeries->append(pos);
    }
}

MemorySnapshot MemoryVisualizerWidget::takeMemorySnapshot() {
    MemorySnapshot snapshot;
    
    // Get current memory state
    auto& memorySystem = MemorySystem::Get();
    snapshot.totalUsed = memorySystem.GetManager().GetTotalAllocated();
    snapshot.peakUsage = memorySystem.GetManager().GetPeakUsage();
    
    // Get allocation details
    #if HD_DEBUG
        auto& allocationMap = memorySystem.GetManager().GetAllocationMap();
        snapshot.allocations.reserve(allocationMap.size());
        for (const auto& [ptr, block] : allocationMap) {
            snapshot.allocations.push_back(block);
        }
    #endif
    
    return snapshot;
}

}} // namespace Hydragon::Tools 