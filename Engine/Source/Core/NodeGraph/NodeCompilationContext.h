/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace hd {

class Node;

class NodeCompilationContext {
public:
    void AddDefaultNodeCompilation(Node* node);
    void AddOptimizedNodeCompilation(Node* node, const std::string& optimizationHint);
    void AddInlinedNodeCompilation(Node* node);
    
    bool ValidateCompilation() const;
    void GenerateCompiledOutput(const std::string& targetPath);

    // Optimization settings
    void SetOptimizationLevel(int level) { optimizationLevel = level; }
    void EnableInlining(bool enable) { inliningEnabled = enable; }
    void SetMemoryLimit(size_t bytes) { memoryLimit = bytes; }

private:
    struct CompilationUnit {
        Node* node;
        std::string optimizationHint;
        bool isInlined;
    };

    std::vector<CompilationUnit> compilationUnits;
    std::unordered_map<Node*, size_t> nodeToUnitMap;
    
    int optimizationLevel = 1;
    bool inliningEnabled = true;
    size_t memoryLimit = SIZE_MAX;
};

} // namespace hd
