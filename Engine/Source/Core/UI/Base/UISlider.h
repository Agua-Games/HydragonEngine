// UISlider.h
//
// UI Slider node mapping directly to ImGui::SliderFloat.

#pragma once
#include "../../NodeGraph/Node.h"
#include <string>

namespace hd {

class UISlider : public Node {
public:
    UISlider(const std::string& name = "", const std::string& label = "Slider", float val = 0.0f, float minVal = 0.0f, float maxVal = 1.0f);
    virtual ~UISlider() override = default;

    virtual void processNode() override;
};

} // namespace hd
