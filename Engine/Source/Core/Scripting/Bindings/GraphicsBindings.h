/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graphics system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Graphics/GraphicsSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct GraphicsBindingsConfig {
    bool enableMaterialScripting = true;
    bool enableShaderHotReload = true;
    bool enableRenderPassScripting = true;
    bool enableDebugVisualization = true;
    uint32_t maxCustomMaterials = 100;
    uint32_t maxCustomShaders = 50;
    uint32_t maxRenderCallbacks = 20;
    std::string shaderCachePath = "Cache/Shaders";
};

class GraphicsBindings {
public:
    static GraphicsBindings& Get();
    
    void Initialize(const GraphicsBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Material management
    MaterialHandle CreateMaterial(const MaterialDesc& desc);
    void DestroyMaterial(MaterialHandle handle);
    void UpdateMaterial(MaterialHandle handle, const MaterialProperties& props);
    
    // Shader management
    ShaderHandle CreateShader(const ShaderDesc& desc);
    void DestroyShader(ShaderHandle handle);
    void ReloadShader(ShaderHandle handle);
    
    // Render pass control
    void RegisterRenderPass(const std::string& name, const RenderPassDesc& desc);
    void UnregisterRenderPass(const std::string& name);
    void SetRenderPassEnabled(const std::string& name, bool enabled);
    
    // Debug visualization
    void DrawDebugLine(const Vector3& start, const Vector3& end, const Color& color);
    void DrawDebugSphere(const Vector3& center, float radius, const Color& color);
    void DrawDebugText(const Vector3& position, const std::string& text, const Color& color);
    
    // Custom render callbacks
    void RegisterRenderCallback(const std::string& name, const RenderCallback& callback);
    void UnregisterRenderCallback(const std::string& name);
    
    const GraphicsBindingsStats& GetStats() const { return m_Stats; }

private:
    GraphicsBindings() = default;
    
    GraphicsBindingsConfig m_Config;
    GraphicsBindingsStats m_Stats;
    std::unordered_map<MaterialHandle, MaterialInstance> m_Materials;
    std::unordered_map<ShaderHandle, ShaderInstance> m_Shaders;
    std::unordered_map<std::string, RenderPassInstance> m_RenderPasses;
    std::unordered_map<std::string, RenderCallback> m_RenderCallbacks;
    std::unique_ptr<Graphics::IShaderCompiler> m_ShaderCompiler;
    std::unique_ptr<Graphics::IMaterialProcessor> m_MaterialProcessor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 