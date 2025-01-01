/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory search system for Hydragon
 */

#include "MemorySearchSystem.h"
#include <QRegularExpression>

namespace Hydragon {
namespace Tools {

std::vector<SearchResult> MemorySearchSystem::search(
    const QString& query, const SearchOptions& options) {
    
    std::vector<SearchResult> results;
    
    // Prepare regex if needed
    std::optional<QRegularExpression> regex;
    if (options.useRegex) {
        QRegularExpression::PatternOptions regexOptions;
        if (!options.caseSensitive) {
            regexOptions |= QRegularExpression::CaseInsensitiveOption;
        }
        regex.emplace(query, regexOptions);
    }
    
    for (const auto& block : m_BlockProvider->getAllBlocks()) {
        SearchResult result{block, 0.0f};
        
        // Try exact match first
        if (matchesExact(block, query, options)) {
            result.matchScore = 1.0f;
            results.push_back(result);
            continue;
        }
        
        // Try regex if enabled
        if (regex && matchesRegex(block, *regex)) {
            result.matchScore = 0.9f;
            results.push_back(result);
            continue;
        }
        
        // Try fuzzy match
        if (options.fuzzyMatch) {
            float fuzzyScore = calculateFuzzyScore(block, query, options);
            if (fuzzyScore > options.fuzzyThreshold) {
                result.matchScore = fuzzyScore;
                results.push_back(result);
            }
        }
    }
    
    // Sort by relevance
    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) {
                  return a.matchScore > b.matchScore;
              });
    
    return results;
}

float MemorySearchSystem::calculateFuzzyScore(
    const Memory::MemoryBlock& block,
    const QString& query,
    const SearchOptions& options) {
    
    float bestScore = 0.0f;
    
    // Check block name/tag
    if (options.searchInTags) {
        bestScore = std::max(bestScore, 
            levenshteinSimilarity(block.tag, query));
    }
    
    // Check address if enabled
    if (options.searchInAddresses) {
        QString addrStr = QString::number((quintptr)block.data, 16);
        bestScore = std::max(bestScore,
            levenshteinSimilarity(addrStr, query));
    }
    
    return bestScore;
}

float MemorySearchSystem::levenshteinSimilarity(
    const QString& s1, const QString& s2) {
    
    const int len1 = s1.length();
    const int len2 = s2.length();
    
    std::vector<std::vector<int>> dp(len1 + 1, 
        std::vector<int>(len2 + 1));
    
    // Fill the matrix
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0) dp[i][j] = j;
            else if (j == 0) dp[i][j] = i;
            else if (s1[i-1] == s2[j-1]) dp[i][j] = dp[i-1][j-1];
            else dp[i][j] = 1 + std::min({dp[i-1][j],    // deletion
                                         dp[i][j-1],      // insertion
                                         dp[i-1][j-1]});  // substitution
        }
    }
    
    // Convert distance to similarity score (0-1)
    int maxLen = std::max(len1, len2);
    if (maxLen == 0) return 1.0f;
    
    return 1.0f - (float)dp[len1][len2] / maxLen;
}

float MemorySearchSystem::calculateEnhancedFuzzyScore(
    const QString& text, const QString& pattern) {
    
    float levenScore = levenshteinSimilarity(text, pattern);
    float soundexScore = soundexSimilarity(text, pattern);
    float ngramScore = ngramSimilarity(text, pattern, 3);  // trigrams
    
    // Weighted combination
    return levenScore * 0.5f + 
           soundexScore * 0.3f + 
           ngramScore * 0.2f;
}

float MemorySearchSystem::soundexSimilarity(const QString& s1, const QString& s2) {
    static const std::unordered_map<QChar, char> soundexMap = {
        {'B','1'}, {'F','1'}, {'P','1'}, {'V','1'},
        {'C','2'}, {'G','2'}, {'J','2'}, {'K','2'}, {'Q','2'},
        {'S','2'}, {'X','2'}, {'Z','2'},
        {'D','3'}, {'T','3'},
        {'L','4'},
        {'M','5'}, {'N','5'},
        {'R','6'}
    };
    
    QString code1 = generateSoundex(s1, soundexMap);
    QString code2 = generateSoundex(s2, soundexMap);
    
    return code1 == code2 ? 1.0f : 0.0f;
}

}} // namespace Hydragon::Tools 