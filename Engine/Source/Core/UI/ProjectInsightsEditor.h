/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProjectInsightsEditor.h
 * @brief The Project Insights Editor is a specialized view for project analysis and insights.
 */
#pragma once

namespace hd {

struct ProjectInsightsState {
    // Performance checks
    bool memoryAnalysis = false;
    bool cpuProfiling = false;
    bool gpuResourceMgmt = false;
    bool assetLoadingOpt = false;

    // Best practices
    bool codeStyleCheck = false;
    bool projectStructure = false;
    bool namingConventions = false;
    bool documentationCoverage = false;

    // Workflows
    bool buildPipelineAnalysis = false;
    bool versionControlUsage = false;
    bool teamCollabPatterns = false;

    // Analysis state
    bool isAnalyzing = false;
    float analysisProgress = 0.0f;
};

struct EditorWindowData;
void ShowProjectInsightsEditor(bool* p_open, EditorWindowData* windowData);

} // namespace hd
