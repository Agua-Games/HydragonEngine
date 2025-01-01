#pragma once
#include <QMainWindow>
#include "MemoryVisualizerWidget.h"

namespace Hydragon {
namespace Tools {

class MemoryVisualizerWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MemoryVisualizerWindow(QWidget* parent = nullptr);

private:
    MemoryVisualizerWidget* m_VisualizerWidget;
    
    void setupMenus();
    void setupDockWidgets();
    
    // Export/Import
    void saveSnapshot();
    void loadSnapshot();
};

}} // namespace Hydragon::Tools 