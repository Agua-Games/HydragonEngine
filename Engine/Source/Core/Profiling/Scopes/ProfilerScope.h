/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * RAII profiler scope management
 */

#pragma once
#include "../ProfilerTypes.h"
#include <string>

namespace Hydragon::Profiling {

class ProfilerScope {
public:
    ProfilerScope(const char* name, ProfilerCategory category = ProfilerCategory::CPU)
        : m_Name(name)
        , m_Category(category) {
        m_Handle = ProfilerSystem::Get().BeginScope(name, category);
    }
    
    ~ProfilerScope() {
        ProfilerSystem::Get().EndScope(m_Handle);
    }
    
    ProfilerScope(const ProfilerScope&) = delete;
    ProfilerScope& operator=(const ProfilerScope&) = delete;

private:
    const char* m_Name;
    ProfilerCategory m_Category;
    ScopeHandle m_Handle;
};

#define PROFILE_SCOPE(name) ProfilerScope CONCAT(scope_, __LINE__)(name)
#define PROFILE_FUNCTION() ProfilerScope CONCAT(scope_, __LINE__)(__FUNCTION__)
#define PROFILE_GPU_SCOPE(name) ProfilerGPUScope CONCAT(gpuScope_, __LINE__)(name)

} // namespace Hydragon::Profiling 