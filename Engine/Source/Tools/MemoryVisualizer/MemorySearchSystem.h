#pragma once
#include <QObject>
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class MemorySearchSystem : public QObject {
    Q_OBJECT
public:
    struct SearchResult {
        const Memory::MemoryBlock* block;
        float matchScore;  // 0-1 relevance
        std::vector<std::string> matchedTerms;
    };

    struct SearchOptions {
        bool caseSensitive = false;
        bool searchInTags = true;
        bool searchInAddresses = true;
        bool fuzzyMatch = true;
    };

    std::vector<SearchResult> search(const QString& query, 
                                   const SearchOptions& options = SearchOptions());
    
signals:
    void searchCompleted(const std::vector<SearchResult>& results);
    void resultSelected(const Memory::MemoryBlock* block);

private:
    float calculateRelevance(const Memory::MemoryBlock& block, 
                           const QString& query,
                           const SearchOptions& options);
    
    bool matchesPattern(const QString& text, 
                       const QString& pattern,
                       const SearchOptions& options);

    // Cache structures
    struct SearchCache {
        QString lastQuery;
        SearchOptions lastOptions;
        std::vector<SearchResult> cachedResults;
        std::chrono::steady_clock::time_point timestamp;
    };
    
    // Indexing structures
    struct SearchIndex {
        std::unordered_map<std::string, std::vector<const Memory::MemoryBlock*>> tagIndex;
        std::unordered_map<size_t, std::vector<const Memory::MemoryBlock*>> sizeIndex;
        std::vector<std::pair<uintptr_t, const Memory::MemoryBlock*>> addressIndex;
    };
    
    SearchCache m_Cache;
    SearchIndex m_Index;
    
    void updateSearchIndex();
    void pruneCache();
};

}} // namespace Hydragon::Tools 