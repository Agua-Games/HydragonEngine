/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Build dependency tracking and analysis
 */

#pragma once
#include "../BuildTypes.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::DevTools {

class DependencyTracker {
public:
    static DependencyTracker& Get();
    
    void Initialize();
    void Shutdown();
    
    void AddDependency(const std::string& target, const std::string& dependency);
    void RemoveDependency(const std::string& target, const std::string& dependency);
    
    std::vector<std::string> GetDependencies(const std::string& target) const;
    std::vector<std::string> GetDependents(const std::string& target) const;
    
    bool HasCircularDependencies(const std::string& target) const;
    std::vector<std::string> GetBuildOrder(const std::string& target) const;
    
    void Clear();
    
    const DependencyStats& GetStats() const { return m_Stats; }

private:
    DependencyTracker() = default;
    
    struct DependencyNode {
        std::vector<std::string> dependencies;
        std::vector<std::string> dependents;
        bool visited = false;
        bool inStack = false;
    };

    std::unordered_map<std::string, DependencyNode> m_DependencyGraph;
    DependencyStats m_Stats;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools 