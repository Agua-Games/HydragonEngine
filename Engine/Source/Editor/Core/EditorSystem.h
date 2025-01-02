/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core editor system and framework
 */

#pragma once
#include "EditorTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct EditorConfig {
    bool enableUndoRedo = true;
    bool enableAutoSave = true;
    bool enableSnapToGrid = true;
    bool enableLivePreview = true;
    uint32_t maxUndoSteps = 100;
    uint32_t autoSaveInterval = 300;  // 5 minutes
    std::string projectPath;
    std::string tempDirectory = "EditorTemp";
};

class EditorSystem {
public:
    static EditorSystem& Get();
    
    void Initialize(const EditorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void OpenProject(const std::string& path);
    void SaveProject();
    void CloseProject();
    
    void Undo();
    void Redo();
    
    void BeginEditOperation(const std::string& name);
    void EndEditOperation();
    
    void RegisterTool(const EditorTool& tool);
    void UnregisterTool(const std::string& name);
    
    void SetActiveTool(const std::string& name);
    EditorTool* GetActiveTool() const;
    
    const EditorStats& GetStats() const { return m_Stats; }

private:
    EditorSystem() = default;
    
    EditorConfig m_Config;
    EditorStats m_Stats;
    std::vector<EditorTool> m_Tools;
    std::unique_ptr<IEditorContext> m_Context;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 