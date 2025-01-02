/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Build pipeline and task management
 */

#pragma once
#include "../BuildTypes.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::DevTools {

class IBuildPipeline {
public:
    virtual ~IBuildPipeline() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void AddTask(const BuildTask& task) = 0;
    virtual void RemoveTask(const std::string& taskName) = 0;
    
    virtual bool ExecutePipeline() = 0;
    virtual void CancelPipeline() = 0;
    
    virtual bool IsRunning() const = 0;
    virtual float GetProgress() const = 0;
    
    virtual const PipelineStats& GetStats() const = 0;
};

class BuildPipeline : public IBuildPipeline {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void AddTask(const BuildTask& task) override;
    void RemoveTask(const std::string& taskName) override;
    
    bool ExecutePipeline() override;
    void CancelPipeline() override;
    
    bool IsRunning() const override;
    float GetProgress() const override;
    
    const PipelineStats& GetStats() const override;

private:
    std::vector<BuildTask> m_Tasks;
    std::unique_ptr<ITaskExecutor> m_Executor;
    PipelineStats m_Stats;
    bool m_Running = false;
};

} // namespace Hydragon::DevTools 