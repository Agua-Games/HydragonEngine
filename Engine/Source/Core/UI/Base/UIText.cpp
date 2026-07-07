// UIText.cpp
//
// Implementation of the UIText class. Maps to ImGui::Text.

#include "UIText.h"
#include <imgui.h>

namespace hd {

UIText::UIText(const std::string& name, const std::string& text)
    : Node(name) {
    setProperty<std::string>("Text", text);
}

void UIText::processNode() {
    std::string text = getProperty<std::string>("Text", "");
    
    // Draw the static or dynamic text layout
    ImGui::TextUnformatted(text.c_str());

    // Process potential children
    for (auto& child : getChildren()) {
        child->processNode();
    }
}

} // namespace hd
