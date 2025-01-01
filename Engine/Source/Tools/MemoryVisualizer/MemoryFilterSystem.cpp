#include "MemoryFilterSystem.h"
#include <QRegularExpression>

namespace Hydragon {
namespace Tools {

void MemoryFilterSystem::setFilter(const FilterCriteria& filter) {
    m_CurrentFilter = filter;
    emit filterChanged();
}

bool MemoryFilterSystem::passesFilter(const Memory::MemoryBlock& block) const {
    // Basic filters
    if (!checkSizeConstraints(block)) return false;
    if (!checkTypeConstraints(block)) return false;
    if (!checkTagConstraints(block)) return false;
    
    // Enhanced filters
    if (!checkLifetimeConstraints(block)) return false;
    if (!checkAccessPatternConstraints(block)) return false;
    if (!checkSourceConstraints(block)) return false;
    if (!checkAlignmentConstraints(block)) return false;
    
    return true;
}

bool MemoryFilterSystem::checkLifetimeConstraints(const Memory::MemoryBlock& block) const {
    if (!m_CurrentFilter.lifetimeFilter.enabled) return true;
    
    auto now = std::chrono::steady_clock::now();
    auto age = now - block.allocationTime;
    
    switch (m_CurrentFilter.lifetimeFilter.category) {
        case LifetimeCategory::Recent:
            return age <= std::chrono::minutes(5);
        case LifetimeCategory::LongLived:
            return age >= std::chrono::hours(1);
        default:
            return true;
    }
}

bool MemoryFilterSystem::checkAccessPatternConstraints(const Memory::MemoryBlock& block) const {
    if (!m_CurrentFilter.accessPatternFilter.enabled) return true;
    
    const auto& stats = block.accessStats;
    float accessFrequency = stats.accessCount / stats.lifetimeDuration;
    
    return accessFrequency >= m_CurrentFilter.accessPatternFilter.minFrequency;
}

}} // namespace Hydragon::Tools 