/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Comparison dialog for Hydragon
 */

#pragma once
#include <QDialog>
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class ComparisonDialog : public QDialog {
    Q_OBJECT
public:
    explicit ComparisonDialog(QWidget* parent = nullptr);
    void setBlocks(const Memory::MemoryBlock* a, const Memory::MemoryBlock* b);

private:
    struct ComparisonMetrics {
        size_t sizeDiff;
        float fragmentationDiff;
        float utilizationDiff;
        std::vector<std::string> commonTags;
    };

    QTableWidget* m_ComparisonTable;
    QChartView* m_MetricsChart;
    
    void setupUI();
    void updateComparison();
    ComparisonMetrics calculateMetrics();
    void createComparisonChart(const ComparisonMetrics& metrics);
};

}} // namespace Hydragon::Tools 