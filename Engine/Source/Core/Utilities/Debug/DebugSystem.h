/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Debug visualization and runtime inspection
 */

#pragma once
#include "Core/Math/MathTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Utilities {

struct DebugConfig {
    bool enableDebugDraw = true;
    bool enableInspector = true;
    bool enableConsole = true;
    bool persistentDrawDuration = 0.0f;
    uint32_t maxDrawCommands = 10000;
};

class DebugSystem {
public:
    static DebugSystem& Get();
    
    void Initialize(const DebugConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    
    void DrawLine(const Vector3& start, const Vector3& end, const Color& color);
    void DrawSphere(const Vector3& center, float radius, const Color& color);
    void DrawBox(const Vector3& center, const Vector3& extents, const Color& color);
    void DrawText(const Vector3& position, const std::string& text, const Color& color);
    
    void AddInspectorValue(const std::string& name, const std::string& value);
    void AddConsoleCommand(const std::string& name, const ConsoleCommandFunc& func);
    
    const DebugStats& GetStats() const { return m_Stats; }

private:
    DebugSystem() = default;
    
    DebugConfig m_Config;
    DebugStats m_Stats;
    std::vector<DebugDrawCommand> m_DrawCommands;
    std::unordered_map<std::string, ConsoleCommandFunc> m_ConsoleCommands;
    bool m_Initialized = false;
};

} // namespace Hydragon::Utilities 