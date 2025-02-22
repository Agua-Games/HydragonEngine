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

class HD_Node;

class HD_NodeCompilationContext {
public:
    void AddDefaultNodeCompilation(HD_Node* node);
    void AddOptimizedNodeCompilation(HD_Node* node, const std::string& optimizationHint);
    void AddInlinedNodeCompilation(HD_Node* node);
    
    bool ValidateCompilation() const;
    void GenerateCompiledOutput(const std::string& targetPath);

    // Optimization settings
    void SetOptimizationLevel(int level) { optimizationLevel = level; }
    void EnableInlining(bool enable) { inliningEnabled = enable; }
    void SetMemoryLimit(size_t bytes) { memoryLimit = bytes; }

private:
    struct CompilationUnit {
        HD_Node* node;
        std::string optimizationHint;
        bool isInlined;
    };

    std::vector<CompilationUnit> compilationUnits;
    std::unordered_map<HD_Node*, size_t> nodeToUnitMap;
    
    int optimizationLevel = 1;
    bool inliningEnabled = true;
    size_t memoryLimit = SIZE_MAX;
};

} // namespace hd
