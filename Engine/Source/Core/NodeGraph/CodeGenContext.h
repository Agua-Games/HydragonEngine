/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace hd {

class HD_Node;

class HD_CodeGenContext {
public:
    void AddProcessCall(HD_Node* node);
    void AddInlinedCode(HD_Node* node, const std::string& code);
    void AddDependency(const std::string& header);
    
    void SetTargetLanguage(const std::string& lang) { targetLanguage = lang; }
    void EnableDebugInfo(bool enable) { debugInfoEnabled = enable; }
    
    std::string GenerateCode() const;
    bool ValidateGeneratedCode() const;

private:
    struct CodeUnit {
        HD_Node* node;
        std::string code;
        bool isInlined;
    };

    std::vector<CodeUnit> codeUnits;
    std::vector<std::string> dependencies;
    std::string targetLanguage = "cpp";
    bool debugInfoEnabled = false;
};

} // namespace hd
