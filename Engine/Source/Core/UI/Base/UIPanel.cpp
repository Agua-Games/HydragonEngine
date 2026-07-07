// UIPanel.cpp
//
// Implementation of the UIPanel class. Maps to ImGui::Begin and ImGui::End.

#include "UIPanel.h"
#include <imgui.h>

namespace hd {

UIPanel::UIPanel(const std::string& name, const std::string& title)
    : Node(name) {
    setProperty<std::string>("Title", title);
}

void UIPanel::processNode() {
    std::string title = getProperty<std::string>("Title", "Panel");
    
    // Draw the window container
    ImGui::Begin(title.c_str());

    // Recursively draw all nested child widgets inside this panel
    for (auto& child : getChildren()) {
        child->processNode();
    }

    ImGui::End();
}

} // namespace hd
