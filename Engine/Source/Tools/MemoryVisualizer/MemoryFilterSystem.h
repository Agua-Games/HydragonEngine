/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory filter system for Hydragon
 */

#pragma once
#include <QObject>
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class MemoryFilterSystem : public QObject {
    Q_OBJECT
public:
    struct FilterCriteria {
        std::optional<size_t> minSize;
        std::optional<size_t> maxSize;
        std::vector<Memory::BlockType> types;
        std::vector<std::string> tags;
        bool showFragmented = true;
    };

    void setFilter(const FilterCriteria& filter);
    bool passesFilter(const Memory::MemoryBlock& block) const;
    
signals:
    void filterChanged();
    void filterApplied(size_t matchingBlocks);

private:
    FilterCriteria m_CurrentFilter;
    
    bool checkSizeConstraints(const Memory::MemoryBlock& block) const;
    bool checkTypeConstraints(const Memory::MemoryBlock& block) const;
    bool checkTagConstraints(const Memory::MemoryBlock& block) const;
};

}} // namespace Hydragon::Tools 