#pragma once
#include <QWidget>
#include <QChartView>
#include "Core/Memory/MemorySystem.h"

namespace Hydragon {
namespace Tools {

class MemoryVisualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit MemoryVisualizerWidget(QWidget* parent = nullptr);

private slots:
    void updateMemoryView();
    void onAllocationSelected(const QPointF& point);
    
private:
    // Charts
    QChartView* m_MemoryUsageChart;
    QChartView* m_AllocationDistributionChart;
    
    // Memory data
    struct MemorySnapshot {
        std::vector<Memory::MemoryBlock> allocations;
        size_t totalUsed;
        size_t peakUsage;
    };
    
    void setupCharts();
    void createMemoryMap();
    void updateAllocationDetails(const Memory::MemoryBlock& block);
};

}} // namespace Hydragon::Tools 