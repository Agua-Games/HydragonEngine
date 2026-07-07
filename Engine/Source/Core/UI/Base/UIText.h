// UIText.h
//
// UI Text node mapping directly to ImGui::Text.

#pragma once
#include "../../NodeGraph/Node.h"
#include <string>

namespace hd {

class UIText : public Node {
public:
    UIText(const std::string& name = "", const std::string& text = "Text");
    virtual ~UIText() override = default;

    virtual void processNode() override;
};

} // namespace hd
