#pragma once
#include <QGraphicsView>
#include "Core/Memory/MemoryTypes.h"

namespace Hydragon {
namespace Tools {

class MemoryHeatmapView : public QGraphicsView {
    Q_OBJECT
public:
    enum class VisualizationMode {
        AllocationFrequency,  // How often memory is allocated/deallocated
        AccessPatterns,       // Read/write frequency
        Fragmentation,        // Shows fragmented regions
        PageUtilization,      // Page-level memory usage
        MemoryPressure,      // Combined pressure metrics
        StreamingActivity     // Streaming memory operations
    };

    struct HeatmapLayer {
        std::vector<std::vector<float>> intensities;
        QColor baseColor;
        float opacity;
        bool visible;
    };

    // Enhanced configuration
    struct ViewConfig {
        VisualizationMode mode = VisualizationMode::AllocationFrequency;
        bool showMultiLayers = false;     // Overlay multiple metrics
        float updateInterval = 0.5f;      // Seconds
        float decayRate = 0.95f;          // Intensity decay per update
        size_t gridResolution = 64;       // Grid size
        bool adaptiveColoring = true;     // Adjust colors based on data range
    };

    void setMode(VisualizationMode mode);
    void toggleLayer(const QString& layerName, bool visible);
    void setViewConfig(const ViewConfig& config);

    // Enhanced visualization features
    void zoomToRegion(const QRectF& region);
    void highlightPattern(const std::vector<void*>& addresses);
    void showTimelineAnimation(const std::vector<MemoryState>& states);

private:
    std::map<QString, HeatmapLayer> m_Layers;
    ViewConfig m_Config;

    void updateLayerIntensities(const MemoryState& state);
    void calculateFragmentationPattern();
    void updatePressureVisualization();
    QColor calculateBlendedColor(const std::vector<HeatmapLayer*>& activeLayers, 
                               size_t x, size_t y);
};

}} // namespace Hydragon::Tools 