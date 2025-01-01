/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory profiler export for Hydragon
 */

#pragma once
#include "MemoryBenchmark.h"
#include <string>

namespace Hydragon {

class MemoryProfilerExport {
public:
    // RenderDoc integration
    struct RenderDocExportConfig {
        bool includeTextureAllocs = true;
        bool includeShaderAllocs = true;
        bool trackResourceLifetime = true;
        std::string captureMarker = "HydragonMemoryCapture";
    };
    
    static bool ExportToRenderDoc(const std::vector<MemoryBenchmark::BenchmarkResult>& results,
                                const RenderDocExportConfig& config = RenderDocExportConfig());
    
    // Shader profiling tools
    struct ShaderProfilerConfig {
        enum class Format {
            NSight,      // NVIDIA Nsight
            RGA,        // AMD Radeon GPU Analyzer
            SPIRV      // SPIR-V Tools
        };
        Format outputFormat = Format::NSight;
        bool includeMemoryLayout = true;
        bool trackUniformAccess = true;
    };
    
    static bool ExportToShaderProfiler(const std::vector<MemoryBenchmark::BenchmarkResult>& results,
                                     const ShaderProfilerConfig& config = ShaderProfilerConfig());

private:
    static void FormatRenderDocMarkers(const BenchmarkResult& result);
    static void GenerateShaderMemoryLayout(const BenchmarkResult& result);
};

} // namespace Hydragon 