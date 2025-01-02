/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Material editing and shader graph system
 */

#pragma once
#include "MaterialTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct MaterialEditorConfig {
    bool enableLivePreview = true;
    bool enableNodePreview = true;
    bool enableShaderDebug = true;
    bool enableMaterialInstancing = true;
    uint32_t maxPreviewResolution = 512;
    uint32_t maxNodeCount = 1000;
    size_t previewMemoryBudget = 128 * 1024 * 1024;  // 128MB
};

class MaterialEditorSystem {
public:
    static MaterialEditorSystem& Get();
    
    void Initialize(const MaterialEditorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void CreateMaterial(const std::string& name);
    void LoadMaterial(const std::string& path);
    void SaveMaterial(const std::string& path);
    
    void AddNode(const NodeDesc& desc);
    void RemoveNode(NodeHandle handle);
    void ConnectNodes(NodeHandle output, NodeHandle input, uint32_t slot);
    
    void SetParameter(const std::string& name, const MaterialParameter& value);
    void UpdatePreview();
    
    void CompileShader();
    void ValidateGraph();
    
    const MaterialEditorStats& GetStats() const { return m_Stats; }

private:
    MaterialEditorSystem() = default;
    
    MaterialEditorConfig m_Config;
    MaterialEditorStats m_Stats;
    std::unique_ptr<IMaterialGraph> m_Graph;
    std::unique_ptr<IShaderCompiler> m_Compiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 