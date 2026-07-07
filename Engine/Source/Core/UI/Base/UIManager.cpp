// UIManager.cpp
//
// Implementation of the UIManager class. Setup ImGui and translate ImGui draw lists to Filament geometry.

#include "UIManager.h"
#include <filament/Engine.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Camera.h>
#include <filament/Texture.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/Viewport.h>
#include <filament/TextureSampler.h>
#include <utils/EntityManager.h>
#include <fstream>
#include <iostream>

// Standard Win32 message handler hook for ImGui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hd {

static std::vector<uint8_t> loadBinaryFile(const std::string& filename) {
    std::vector<std::string> searchPaths = {
        filename,
        "Engine/Source/Core/UI/Base/" + filename,
        "../Engine/Source/Core/UI/Base/" + filename,
        "../../Engine/Source/Core/UI/Base/" + filename,
        "../../../Engine/Source/Core/UI/Base/" + filename
    };

    for (const auto& path : searchPaths) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> buffer(size);
            if (file.read((char*)buffer.data(), size)) {
                return buffer;
            }
        }
    }
    return {};
}

UIManager::UIManager(const std::string& name, filament::Engine* engine)
    : Node(name)
    , m_engine(engine) {
}

UIManager::~UIManager() {
    shutdown();
}

void UIManager::initialize(HWND hwnd, uint32_t width, uint32_t height) {
    m_hwnd = hwnd;
    m_width = width;
    m_height = height;

    if (!m_engine) return;

    // 1. Create separate Scene, View, and Camera for the UI layer
    m_uiScene = m_engine->createScene();
    m_uiView = m_engine->createView();
    m_uiCameraEntity = utils::EntityManager::get().create();
    m_uiCamera = m_engine->createCamera(m_uiCameraEntity);

    m_uiView->setScene(m_uiScene);
    m_uiView->setCamera(m_uiCamera);
    m_uiView->setViewport({ 0, 0, m_width, m_height });
    m_uiView->setPostProcessingEnabled(false);
    m_uiView->setShadowingEnabled(false);
    m_uiView->setBlendMode(filament::View::BlendMode::TRANSLUCENT);

    // Setup UI Camera orthographic projection mapping to screen coordinates
    m_uiCamera->setProjection(filament::Camera::Projection::ORTHO,
        0.0, double(m_width), double(m_height), 0.0, 0.0, 1.0);

    // 2. Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)m_width, (float)m_height);
    io.IniFilename = nullptr; // Avoid writing imgui.ini

    ImGui::StyleColorsDark();

    // 3. Setup GPU assets for ImGui rendering
    initImGuiRenderer();
}

void UIManager::initImGuiRenderer() {
    ImGuiIO& io = ImGui::GetIO();

    // Create the Font Atlas Texture in Filament
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    m_fontTexture = filament::Texture::Builder()
        .width((uint32_t)width)
        .height((uint32_t)height)
        .levels(1)
        .format(filament::Texture::InternalFormat::RGBA8)
        .build(*m_engine);

    m_fontTexture->setImage(*m_engine, 0,
        filament::Texture::PixelBufferDescriptor(pixels, size_t(width * height * 4),
            filament::Texture::Format::RGBA, filament::Texture::Type::UBYTE));

    io.Fonts->SetTexID((ImTextureID)m_fontTexture);

    // Load compiled material package
    auto matBuffer = loadBinaryFile("imgui.filamat");
    if (!matBuffer.empty()) {
        m_uiMaterial = filament::Material::Builder()
            .package(matBuffer.data(), matBuffer.size())
            .build(*m_engine);
        m_uiMaterialInstance = m_uiMaterial->createInstance();
        m_uiMaterialInstance->setParameter("albedo", m_fontTexture,
            filament::TextureSampler(filament::TextureSampler::MinFilter::LINEAR, filament::TextureSampler::MagFilter::LINEAR));
    }

    // Allocate initial buffers (capacity will grow dynamically on demand)
    m_vertexCount = 10000;
    m_indexCount = 15000;

    m_vb = filament::VertexBuffer::Builder()
        .vertexCount((uint32_t)m_vertexCount)
        .bufferCount(1)
        .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2, offsetof(ImDrawVert, pos), sizeof(ImDrawVert))
        .attribute(filament::VertexAttribute::UV0, 0, filament::VertexBuffer::AttributeType::FLOAT2, offsetof(ImDrawVert, uv), sizeof(ImDrawVert))
        .attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4, offsetof(ImDrawVert, col), sizeof(ImDrawVert))
        .normalized(filament::VertexAttribute::COLOR)
        .build(*m_engine);

    filament::IndexBuffer::IndexType indexType = (sizeof(ImDrawIdx) == 2) ?
        filament::IndexBuffer::IndexType::USHORT : filament::IndexBuffer::IndexType::UINT;

    m_ib = filament::IndexBuffer::Builder()
        .indexCount((uint32_t)m_indexCount)
        .bufferType(indexType)
        .build(*m_engine);

    // Create the rendering entity
    m_uiEntity = utils::EntityManager::get().create();

    // Pre-allocate 64 rendering primitives inside the entity
    filament::RenderableManager::Builder builder(64);
    builder.castShadows(false)
        .receiveShadows(false)
        .culling(false)
        .priority(7); // High priority to render on top

    for (int i = 0; i < 64; ++i) {
        builder.material(i, m_uiMaterialInstance)
               .geometry(i, filament::RenderableManager::PrimitiveType::TRIANGLES, m_vb, m_ib, 0, 0);
    }

    builder.build(*m_engine, m_uiEntity);
    m_uiScene->addEntity(m_uiEntity);
}

void UIManager::newFrame(float deltaTime) {
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = deltaTime;
    io.DisplaySize = ImVec2((float)m_width, (float)m_height);

    // Synchronize mouse position and buttons with Win32
    POINT mousePos;
    if (GetCursorPos(&mousePos) && ScreenToClient(m_hwnd, &mousePos)) {
        io.MousePos = ImVec2((float)mousePos.x, (float)mousePos.y);
    }
    io.MouseDown[0] = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
    io.MouseDown[1] = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;

    ImGui::NewFrame();
}

void UIManager::render() {
    // 1. Process all children nodes (which make ImGui widget calls)
    processNode();

    // 2. Render ImGui to generate draw lists
    ImGui::Render();

    // 3. Update the GPU buffers and draw lists
    renderImGuiDrawData(ImGui::GetDrawData());
}

void UIManager::renderImGuiDrawData(ImDrawData* drawData) {
    if (!m_engine || !drawData || drawData->CmdListsCount == 0) return;

    // Collect all vertices and indices across command lists
    m_vertexCache.clear();
    m_indexCache.clear();

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];
        m_vertexCache.insert(m_vertexCache.end(), cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Data + cmd_list->VtxBuffer.Size);
        m_indexCache.insert(m_indexCache.end(), cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Data + cmd_list->IdxBuffer.Size);
    }

    if (m_vertexCache.empty() || m_indexCache.empty()) return;

    // Grow Vertex Buffer dynamically if vertex count exceeds current capacity
    if (m_vertexCache.size() > m_vertexCount) {
        m_vertexCount = m_vertexCache.size() + 5000;
        m_engine->destroy(m_vb);
        m_vb = filament::VertexBuffer::Builder()
            .vertexCount((uint32_t)m_vertexCount)
            .bufferCount(1)
            .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT2, offsetof(ImDrawVert, pos), sizeof(ImDrawVert))
            .attribute(filament::VertexAttribute::UV0, 0, filament::VertexBuffer::AttributeType::FLOAT2, offsetof(ImDrawVert, uv), sizeof(ImDrawVert))
            .attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4, offsetof(ImDrawVert, col), sizeof(ImDrawVert))
            .normalized(filament::VertexAttribute::COLOR)
            .build(*m_engine);
    }

    // Grow Index Buffer dynamically if index count exceeds current capacity
    if (m_indexCache.size() > m_indexCount) {
        m_indexCount = m_indexCache.size() + 10000;
        m_engine->destroy(m_ib);
        filament::IndexBuffer::IndexType indexType = (sizeof(ImDrawIdx) == 2) ?
            filament::IndexBuffer::IndexType::USHORT : filament::IndexBuffer::IndexType::UINT;
        m_ib = filament::IndexBuffer::Builder()
            .indexCount((uint32_t)m_indexCount)
            .bufferType(indexType)
            .build(*m_engine);
    }

    // Upload vertices and indices to the GPU buffers
    m_vb->setBufferAt(*m_engine, 0,
        filament::VertexBuffer::BufferDescriptor(m_vertexCache.data(), m_vertexCache.size() * sizeof(ImDrawVert)));

    m_ib->setBuffer(*m_engine,
        filament::IndexBuffer::BufferDescriptor(m_indexCache.data(), m_indexCache.size() * sizeof(ImDrawIdx)));

    auto& rcm = m_engine->getRenderableManager();
    auto instance = rcm.getInstance(m_uiEntity);

    // Bind primitives dynamically to geometry slices
    int primIndex = 0;
    uint32_t indexOffset = 0;
    uint32_t vertexOffset = 0;

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                if (primIndex < 64) {
                    // Update render range for this draw command
                    rcm.setGeometryAt(instance, primIndex,
                        filament::RenderableManager::PrimitiveType::TRIANGLES,
                        m_vb, m_ib,
                        indexOffset + pcmd->IdxOffset, pcmd->ElemCount);
                    primIndex++;
                }
            }
        }
        indexOffset += cmd_list->IdxBuffer.Size;
        vertexOffset += cmd_list->VtxBuffer.Size;
    }

    // Set remaining primitives to draw nothing
    for (int i = primIndex; i < 64; ++i) {
        rcm.setGeometryAt(instance, i,
            filament::RenderableManager::PrimitiveType::TRIANGLES,
            m_vb, m_ib, 0, 0);
    }
}

void UIManager::resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    if (m_uiView) {
        m_uiView->setViewport({ 0, 0, m_width, m_height });
    }
    if (m_uiCamera) {
        m_uiCamera->setProjection(filament::Camera::Projection::ORTHO,
            0.0, double(m_width), double(m_height), 0.0, 0.0, 1.0);
    }
}

void UIManager::processNode() {
    // Process child nodes recursively
    for (auto& child : getChildren()) {
        child->processNode();
    }
}

void UIManager::shutdown() {
    if (ImGui::GetCurrentContext()) {
        ImGui::DestroyContext();
    }

    if (m_engine) {
        if (m_vb) m_engine->destroy(m_vb);
        if (m_ib) m_engine->destroy(m_ib);
        if (m_fontTexture) m_engine->destroy(m_fontTexture);
        if (m_uiMaterialInstance) m_engine->destroy(m_uiMaterialInstance);
        if (m_uiMaterial) m_engine->destroy(m_uiMaterial);
        
        if (!m_uiCameraEntity.isNull()) {
            m_engine->destroyCameraComponent(m_uiCameraEntity);
            utils::EntityManager::get().destroy(m_uiCameraEntity);
        }

        if (!m_uiEntity.isNull()) {
            m_engine->destroy(m_uiEntity);
            utils::EntityManager::get().destroy(m_uiEntity);
        }

        if (m_uiScene) m_engine->destroy(m_uiScene);
        if (m_uiView) m_engine->destroy(m_uiView);

        m_vb = nullptr;
        m_ib = nullptr;
        m_fontTexture = nullptr;
        m_uiMaterialInstance = nullptr;
        m_uiMaterial = nullptr;
        m_uiCamera = nullptr;
        m_uiScene = nullptr;
        m_uiView = nullptr;
        m_engine = nullptr;
    }
}

} // namespace hd
