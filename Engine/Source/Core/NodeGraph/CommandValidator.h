/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * Class for validating command inputs
 */
#pragma once
#include <functional>
#include <string>
#include <vector>
#include <any>
#include <unordered_map>

namespace hd {

/**
 * @brief Validation rule for command inputs
 */
struct ValidationRule {
    std::function<bool(const std::any&)> validate;
    std::string errorMessage;
};

class HD_CommandValidator {
public:
    template<typename T>
    void AddRule(const std::string& inputName, 
                std::function<bool(const T&)> rule,
                const std::string& errorMsg) {
        rules[inputName].push_back({
            [rule](const std::any& value) {
                return rule(std::any_cast<T>(value));
            },
            errorMsg
        });
    }

    bool Validate(const std::string& inputName, const std::any& value) {
        if (auto it = rules.find(inputName); it != rules.end()) {
            for (const auto& rule : it->second) {
                if (!rule.validate(value)) {
                    lastError = rule.errorMessage;
                    return false;
                }
            }
        }
        return true;
    }

    const std::string& GetLastError() const { return lastError; }

private:
    std::unordered_map<std::string, std::vector<ValidationRule>> rules;
    std::string lastError;
};

} // namespace hd