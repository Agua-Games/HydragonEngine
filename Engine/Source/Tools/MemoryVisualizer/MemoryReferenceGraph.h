/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory reference graph for Hydragon
 */

#pragma once
#include <QGraphicsScene>
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class MemoryReferenceGraph : public QGraphicsScene {
    Q_OBJECT
public:
    struct ReferenceNode {
        const Memory::MemoryBlock* block;
        QGraphicsItem* visual;
        std::vector<ReferenceNode*> references;
        float referenceStrength;  // How strongly related
    };

    // Visualization modes
    enum class LayoutMode {
        Hierarchical,    // Parent-child relationships
        Force,          // Force-directed graph
        Circular,       // Circular layout
        Timeline        // Time-based relationships
    };

    void setLayoutMode(LayoutMode mode);
    void updateReferences(const std::vector<Memory::MemoryBlock>& blocks);
    
    // Analysis
    void findStrongReferences();  // Identify strongly connected blocks
    void detectCycles();          // Find circular references
    void highlightGroup(const ReferenceNode* node);  // Show related blocks

private:
    std::vector<ReferenceNode> m_Nodes;
    LayoutMode m_CurrentLayout;
    
    void calculateLayout();
    void drawConnections();
    void updateNodeVisuals();
};

}} // namespace Hydragon::Tools 