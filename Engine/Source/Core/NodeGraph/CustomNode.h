/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * CustomNode represents a user-defined node in the engine's node graph.
 */
#pragma once

namespace hd {

class CustomNode : public Node {
    YAML::Node m_Template;
    std::unique_ptr<ScriptEngine> m_ScriptEngine;

public:
    void initialize(const YAML::Node& template) {       // TODO: Move to implementation file
        m_Template = template;
        setupFromTemplate();
    }

    void processNode() override {                           // TODO: Move to implementation file
        if (m_Template["implementation"]["language"].as<std::string>() == "cpp") {
            executeCompiledCode();
        } else {
            m_ScriptEngine->execute(m_Template["implementation"]["process_function"]);
        }
    }
};

} // namespace hd