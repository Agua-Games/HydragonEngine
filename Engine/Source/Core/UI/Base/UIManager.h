// UIManager.h
//
// Root Manager Node for the UI subsystem. Coordinates the ImGui context,
// input event translation, and custom rendering to a separate Filament View.

#pragma once
#include "../../NodeGraph/Node.h"
#include <imgui.h>
#include <utils/Entity.h>
#include <vector>
#include <memory>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace filament {
class Engine;
class View;
class Scene;
class Camera;
class Texture;
class Material;
class MaterialInstance;
class VertexBuffer;
class IndexBuffer;
}

namespace hd {

class UIManager : public Node {
public:
    UIManager(const std::string& name = "", filament::Engine* engine = nullptr);
    virtual ~UIManager() override;

    // Lifecycle Management
    void initialize(HWND hwnd, uint32_t width, uint32_t height);
    void newFrame(float deltaTime);
    void render();
    void shutdown();

    // Accessors
    filament::View* getUIView() const { return m_uiView; }
    void resize(uint32_t width, uint32_t height);

    // Dynamic processing
    virtual void processNode() override;

private:
    void initImGuiRenderer();
    void renderImGuiDrawData(ImDrawData* drawData);

    filament::Engine* m_engine = nullptr;
    HWND m_hwnd = nullptr;
    uint32_t m_width = 0;
    uint32_t m_height = 0;

    // Viewport and Rendering context for UI overlay
    filament::Scene* m_uiScene = nullptr;
    filament::View* m_uiView = nullptr;
    filament::Camera* m_uiCamera = nullptr;
    utils::Entity m_uiCameraEntity;

    // ImGui GPU assets
    filament::Texture* m_fontTexture = nullptr;
    filament::Material* m_uiMaterial = nullptr;
    filament::MaterialInstance* m_uiMaterialInstance = nullptr;
    filament::VertexBuffer* m_vb = nullptr;
    filament::IndexBuffer* m_ib = nullptr;
    utils::Entity m_uiEntity;

    // Dynamic buffer capacity limits
    size_t m_vertexCount = 0;
    size_t m_indexCount = 0;

    // Cache of buffer data to minimize reallocations
    std::vector<ImDrawVert> m_vertexCache;
    std::vector<ImDrawIdx> m_indexCache;
};

} // namespace hd
