// UISlider.cpp
//
// Implementation of the UISlider class. Maps to ImGui::SliderFloat.

#include "UISlider.h"
#include <imgui.h>

namespace hd {

UISlider::UISlider(const std::string& name, const std::string& label, float val, float minVal, float maxVal)
    : Node(name) {
    setProperty<std::string>("Label", label);
    setProperty<float>("Value", val);
    setProperty<float>("Min", minVal);
    setProperty<float>("Max", maxVal);
}

void UISlider::processNode() {
    std::string label = getProperty<std::string>("Label", "Slider");
    float value = getProperty<float>("Value", 0.0f);
    float minVal = getProperty<float>("Min", 0.0f);
    float maxVal = getProperty<float>("Max", 1.0f);
    
    // Draw the slider and update the Value property feedback
    if (ImGui::SliderFloat(label.c_str(), &value, minVal, maxVal)) {
        setProperty<float>("Value", value);
    }

    // Process potential children
    for (auto& child : getChildren()) {
        child->processNode();
    }
}

} // namespace hd
