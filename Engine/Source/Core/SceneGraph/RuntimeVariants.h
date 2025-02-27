/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace hd {

/**
 * @brief Represents a variant selection criteria
 */
struct VariantCriteria {
    float performanceThreshold = 0.0f;  // Performance-based selection threshold
    float distanceThreshold = 0.0f;     // Distance-based selection threshold
    bool isActive = true;               // Whether this variant is currently active
    
    // Custom criteria can be added by users
    std::unordered_map<std::string, float> customCriteria;
};

/**
 * @brief Represents a single variant of a node or subtree
 */
class HD_Variant {
public:
    explicit HD_Variant(const std::string& name = "", 
                       const VariantCriteria& criteria = VariantCriteria())
        : name(name), criteria(criteria) {}

    const std::string& GetName() const { return name; }
    const VariantCriteria& GetCriteria() const { return criteria; }
    void SetCriteria(const VariantCriteria& newCriteria) { criteria = newCriteria; }

    // Custom criteria management
    void AddCustomCriterion(const std::string& name, float value) {
        criteria.customCriteria[name] = value;
    }

    float GetCustomCriterion(const std::string& name) const {
        auto it = criteria.customCriteria.find(name);
        return it != criteria.customCriteria.end() ? it->second : 0.0f;
    }

private:
    std::string name;
    VariantCriteria criteria;
};

/**
 * @brief Manages a set of variants for a node or subtree
 */
class HD_VariantSet {
public:
    void AddVariant(const std::string& name, const VariantCriteria& criteria) {
        variants.emplace_back(std::make_shared<HD_Variant>(name, criteria));
    }

    std::shared_ptr<HD_Variant> GetVariant(const std::string& name) {
        auto it = std::find_if(variants.begin(), variants.end(),
            [&name](const auto& variant) { return variant->GetName() == name; });
        return it != variants.end() ? *it : nullptr;
    }

    std::shared_ptr<HD_Variant> SelectBestVariant(
        float performanceMetric,
        float distance,
        const std::unordered_map<std::string, float>& customMetrics = {}) 
    {
        std::shared_ptr<HD_Variant> bestVariant = nullptr;
        float bestScore = std::numeric_limits<float>::lowest();

        for (const auto& variant : variants) {
            const auto& criteria = variant->GetCriteria();
            if (!criteria.isActive) continue;

            // Calculate score based on multiple factors
            float score = CalculateVariantScore(
                criteria, performanceMetric, distance, customMetrics);

            if (score > bestScore) {
                bestScore = score;
                bestVariant = variant;
            }
        }

        return bestVariant;
    }

    // Runtime optimization: pre-sort variants by criteria
    void OptimizeForRuntime() {
        std::sort(variants.begin(), variants.end(),
            [](const auto& a, const auto& b) {
                return a->GetCriteria().performanceThreshold < 
                       b->GetCriteria().performanceThreshold;
            });
    }

    const std::vector<std::shared_ptr<HD_Variant>>& GetVariants() const { 
        return variants; 
    }

private:
    std::vector<std::shared_ptr<HD_Variant>> variants;

    float CalculateVariantScore(
        const VariantCriteria& criteria,
        float performanceMetric,
        float distance,
        const std::unordered_map<std::string, float>& customMetrics) 
    {
        // Base score from performance and distance
        float score = 0.0f;
        
        // Performance score (inverse relationship)
        if (performanceMetric <= criteria.performanceThreshold) {
            score += 1.0f - (performanceMetric / criteria.performanceThreshold);
        }
        
        // Distance score (inverse relationship)
        if (distance <= criteria.distanceThreshold) {
            score += 1.0f - (distance / criteria.distanceThreshold);
        }

        // Custom criteria scores
        for (const auto& [name, threshold] : criteria.customCriteria) {
            auto it = customMetrics.find(name);
            if (it != customMetrics.end()) {
                float metric = it->second;
                if (metric <= threshold) {
                    score += 1.0f - (metric / threshold);
                }
            }
        }

        return score;
    }
};

} // namespace hd