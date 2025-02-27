/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * HD_CustomNode represents a user-defined node in the engine's node graph.
 */
#pragma once

namespace hd {

class HD_CustomNode : public HD_Node {
    YAML::Node m_Template;
    std::unique_ptr<ScriptEngine> m_ScriptEngine;

public:
    void Initialize(const YAML::Node& template) {       // TODO: Move to implementation file
        m_Template = template;
        SetupFromTemplate();
    }

    void Process() override {                           // TODO: Move to implementation file
        if (m_Template["implementation"]["language"].as<std::string>() == "cpp") {
            ExecuteCompiledCode();
        } else {
            m_ScriptEngine->Execute(m_Template["implementation"]["process_function"]);
        }
    }
};

} // namespace hd