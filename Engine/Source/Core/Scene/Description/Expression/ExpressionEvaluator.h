/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Expression evaluation for scene descriptions
 */

#pragma once
#include "../SceneDescriptionTypes.h"
#include <memory>
#include <string>

namespace Hydragon::Scene {

class IExpressionEvaluator {
public:
    virtual ~IExpressionEvaluator() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual Parameter EvaluateExpression(const Expression& expr) = 0;
    virtual bool ValidateExpression(const Expression& expr) = 0;
    
    virtual void RegisterFunction(const std::string& name, const Function& func) = 0;
    virtual void UnregisterFunction(const std::string& name) = 0;
};

class ExpressionEvaluator : public IExpressionEvaluator {
public:
    void Initialize() override;
    void Shutdown() override;
    
    Parameter EvaluateExpression(const Expression& expr) override;
    bool ValidateExpression(const Expression& expr) override;
    
    void RegisterFunction(const std::string& name, const Function& func) override;
    void UnregisterFunction(const std::string& name) override;

private:
    struct Context {
        std::unordered_map<std::string, Function> functions;
        std::vector<Parameter> stack;
    };

    Context m_Context;
    std::unique_ptr<IExpressionParser> m_Parser;
};

} // namespace Hydragon::Scene 