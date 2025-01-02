/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Streaming visualizer for Hydragon
 */

class StreamingVisualizer {
public:
    enum class VisualizationMode {
        FlowGraph,    // Shows resource flow from disk → memory
        Timeline,     // Time-based view of streaming operations
        HeatMap      // Shows "hot" areas of memory with high streaming activity
    };
    
    struct StreamingBlock {
        QString resourceName;
        ResourceState state;  // Loading, Ready, Unloading
        float progress;      // 0-1 for streaming progress
        QColor stateColor;   // Visual indicator of state
        
        // Animation properties
        QPropertyAnimation* flowAnimation;
        QParallelAnimationGroup* stateTransition;
    };
    
    // Visual effects for streaming operations
    void animateResourceFlow(const StreamingBlock& block);
    void updateStreamingState(const StreamingBlock& block);
    void highlightActiveStreams();
}; 