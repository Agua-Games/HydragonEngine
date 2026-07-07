// UIButton.h
//
// UI Button node mapping directly to ImGui::Button.

#pragma once
#include "../../NodeGraph/Node.h"
#include <string>

namespace hd {

class UIButton : public Node {
public:
    UIButton(const std::string& name = "", const std::string& text = "Button");
    virtual ~UIButton() override = default;

    virtual void processNode() override;
};

} // namespace hd
