/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene description processing and evaluation
 */

#pragma once
#include "../SceneDescriptionTypes.h"
#include <memory>
#include <string>

namespace Hydragon::Scene {

class IDescriptionProcessor {
public:
    virtual ~IDescriptionProcessor() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void ProcessNode(NodeHandle handle) = 0;
    virtual void EvaluateNode(NodeHandle handle) = 0;
    
    virtual void ValidateGraph() = 0;
    virtual void OptimizeGraph() = 0;
    
    virtual void ClearCache() = 0;
    virtual const ProcessorStats& GetStats() const = 0;
};

class DescriptionProcessor : public IDescriptionProcessor {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void ProcessNode(NodeHandle handle) override;
    void EvaluateNode(NodeHandle handle) override;
    
    void ValidateGraph() override;
    void OptimizeGraph() override;
    
    void ClearCache() override;
    const ProcessorStats& GetStats() const override;

private:
    struct Cache {
        std::unordered_map<NodeHandle, Parameter> evaluationCache;
        std::unordered_map<NodeHandle, bool> validationCache;
    };

    ProcessorStats m_Stats;
    Cache m_Cache;
    std::unique_ptr<IExpressionEvaluator> m_Evaluator;
};

} // namespace Hydragon::Scene 