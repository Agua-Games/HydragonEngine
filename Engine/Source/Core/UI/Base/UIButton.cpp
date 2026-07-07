// UIButton.cpp
//
// Implementation of the UIButton class. Maps to ImGui::Button.

#include "UIButton.h"
#include <imgui.h>

namespace hd {

UIButton::UIButton(const std::string& name, const std::string& text)
    : Node(name) {
    setProperty<std::string>("Text", text);
    setProperty<bool>("Clicked", false);
}

void UIButton::processNode() {
    std::string text = getProperty<std::string>("Text", "Button");
    
    // Draw the button and update the Clicked property feedback
    bool clicked = ImGui::Button(text.c_str());
    setProperty<bool>("Clicked", clicked);

    // Process potential children
    for (auto& child : getChildren()) {
        child->processNode();
    }
}

} // namespace hd
