/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RuntimeVariants.h
 * @brief Header file for the RuntimeVariants classes.
 * 
 * ARCHITECTURAL NOTES:
 * - RuntimeVariants is a utility class for managing runtime variants of nodes or subtrees.
 * 
 * TODO:
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Consider adding support for custom criteria and metrics.
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
class Variant {
public:
    explicit Variant(const std::string& name = "", 
                       const VariantCriteria& criteria = VariantCriteria())
        : name(name), criteria(criteria) {}

    const std::string& getName() const { return name; }
    const VariantCriteria& getCriteria() const { return criteria; }
    void setCriteria(const VariantCriteria& newCriteria) { criteria = newCriteria; }

    // Custom criteria management
    void addCustomCriterion(const std::string& name, float value) {
        criteria.customCriteria[name] = value;
    }

    float getCustomCriterion(const std::string& name) const {
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
class VariantSet {
public:
    void addVariant(const std::string& name, const VariantCriteria& criteria) {
        variants.emplace_back(std::make_shared<Variant>(name, criteria));
    }

    std::shared_ptr<Variant> getVariant(const std::string& name) {
        auto it = std::find_if(variants.begin(), variants.end(),
            [&name](const auto& variant) { return variant->getName() == name; });
        return it != variants.end() ? *it : nullptr;
    }

    std::shared_ptr<Variant> selectBestVariant(
        float performanceMetric,
        float distance,
        const std::unordered_map<std::string, float>& customMetrics = {}) 
    {
        std::shared_ptr<Variant> bestVariant = nullptr;
        float bestScore = std::numeric_limits<float>::lowest();

        for (const auto& variant : variants) {
            const auto& criteria = variant->getCriteria();
            if (!criteria.isActive) continue;

            // Calculate score based on multiple factors
            float score = calculateVariantScore(
                criteria, performanceMetric, distance, customMetrics);

            if (score > bestScore) {
                bestScore = score;
                bestVariant = variant;
            }
        }

        return bestVariant;
    }

    // Runtime optimization: pre-sort variants by criteria
    void optimizeForRuntime() {
        std::sort(variants.begin(), variants.end(),
            [](const auto& a, const auto& b) {
                return a->getCriteria().performanceThreshold < 
                       b->getCriteria().performanceThreshold;
            });
    }

    const std::vector<std::shared_ptr<Variant>>& getVariants() const { 
        return variants; 
    }

private:
    std::vector<std::shared_ptr<Variant>> variants;

    float calculateVariantScore(
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