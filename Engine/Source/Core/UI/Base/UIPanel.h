// UIPanel.h
//
// Container node that represents a UI Window panel (using ImGui::Begin/End).

#pragma once
#include "../../NodeGraph/Node.h"
#include <string>

namespace hd {

class UIPanel : public Node {
public:
    UIPanel(const std::string& name = "", const std::string& title = "Panel");
    virtual ~UIPanel() override = default;

    virtual void processNode() override;
};

} // namespace hd
