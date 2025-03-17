/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
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
    void addDefaultNodeCompilation(Node* node);
    void addOptimizedNodeCompilation(Node* node, const std::string& optimizationHint);
    void addInlinedNodeCompilation(Node* node);
    
    bool validateCompilation() const;
    void generateCompiledOutput(const std::string& targetPath);

    // Optimization settings
    void setOptimizationLevel(int level) { optimizationLevel = level; }
    void enableInlining(bool enable) { inliningEnabled = enable; }
    void setMemoryLimit(size_t bytes) { memoryLimit = bytes; }

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
