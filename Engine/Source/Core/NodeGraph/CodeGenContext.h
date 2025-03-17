/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace hd {

class Node;

class CodeGenContext {
public:
    void addProcessCall(Node* node);
    void addInlinedCode(Node* node, const std::string& code);
    void addDependency(const std::string& header);
    
    void setTargetLanguage(const std::string& lang) { targetLanguage = lang; }
    void enableDebugInfo(bool enable) { debugInfoEnabled = enable; }
    
    std::string generateCode() const;
    bool validateGeneratedCode() const;

private:
    struct CodeUnit {
        Node* node;
        std::string code;
        bool isInlined;
    };

    std::vector<CodeUnit> codeUnits;
    std::vector<std::string> dependencies;
    std::string targetLanguage = "cpp";
    bool debugInfoEnabled = false;
};

} // namespace hd
