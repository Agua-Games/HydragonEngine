// FilamentCubeDemo.cpp
//
// Minimal Win32 front-end that drives the headless HydragonCore to render a colored PBR cube.
// All demo-specific rendering logic remains completely isolated here.

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "../HydragonCore.h"
#include "../../Core/SceneGraph/Scene.h"
#include "../../Core/NodeGraph/Node.h"
#include "../../Core/UI/Base/UIManager.h"
#include "../../Core/UI/Base/UIPanel.h"
#include "../../Core/UI/Base/UIButton.h"
#include "../../Core/UI/Base/UIText.h"
#include "../../Core/UI/Base/UISlider.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>
#include <filament/Skybox.h>
#include <filament/LightManager.h>
#include <filament/Color.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat4.h>
#include <math/mat3.h>
#include <math/quat.h>
#include <utils/EntityManager.h>

using namespace filament::math;

namespace {

constexpr uint32_t kWidth = 1280;
constexpr uint32_t kHeight = 720;

bool gRunning = true;

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }
    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY:
            gRunning = false;
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wparam == VK_ESCAPE) {
                gRunning = false;
                PostQuitMessage(0);
            }
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}

HWND createWindow() {
    HINSTANCE instance = GetModuleHandleW(nullptr);
    WNDCLASSW wc = {};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"HydragonFilamentDemo";
    RegisterClassW(&wc);

    RECT rect = {0, 0, LONG(kWidth), LONG(kHeight)};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    return CreateWindowExW(
        0, wc.lpszClassName, L"Hydragon - Filament Cube Demo (Vulkan)",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, instance, nullptr);
}

// Robust helper to load binary files by testing multiple possible relative paths
std::vector<uint8_t> loadBinaryFile(const std::string& filename) {
    std::vector<std::string> paths = {
        filename,
        "Engine/Source/Runtime/Demo/" + filename,
        "../../../Source/Runtime/Demo/" + filename,
        "../../Source/Runtime/Demo/" + filename,
        "../Engine/Source/Runtime/Demo/" + filename
    };

    for (const auto& path : paths) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<uint8_t> buffer(size);
            if (file.read((char*)buffer.data(), size)) {
                std::printf("[demo] loaded %s from %s (%u bytes)\n",
                            filename.c_str(), path.c_str(), unsigned(size));
                std::fflush(stdout);
                return buffer;
            }
        }
    }
    return {};
}

// Pack an RGBA color into a little-endian UBYTE4 (memory order r,g,b,a).
constexpr uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (uint32_t(a) << 24) | (uint32_t(b) << 16) | (uint32_t(g) << 8) | uint32_t(r);
}

struct Vertex {
    float3 position;
    quatf tangents;
    uint32_t color;
};

// Generates 24 vertices for a cube with correct tangent frames for lighting.
void createCube(Vertex* vertices, uint16_t* indices) {
    auto pack = [](float3 normal, float3 tangent) {
        float3 bitangent = normalize(cross(normal, tangent));
        mat3f m(tangent, bitangent, normal);
        return mat3f::packTangentFrame(m);
    };

    quatf qFront  = pack({ 0.0f,  0.0f,  1.0f}, { 1.0f,  0.0f,  0.0f});
    quatf qBack   = pack({ 0.0f,  0.0f, -1.0f}, {-1.0f,  0.0f,  0.0f});
    quatf qLeft   = pack({-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f,  1.0f});
    quatf qRight  = pack({ 1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, -1.0f});
    quatf qTop    = pack({ 0.0f,  1.0f,  0.0f}, { 1.0f,  0.0f,  0.0f});
    quatf qBottom = pack({ 0.0f, -1.0f,  0.0f}, {-1.0f,  0.0f,  0.0f});

    uint32_t cRed     = packColor(220,  50,  50);
    uint32_t cGreen   = packColor( 50, 180,  50);
    uint32_t cBlue    = packColor( 50,  50, 220);
    uint32_t cYellow  = packColor(220, 220,  50);
    uint32_t cMagenta = packColor(220,  50, 220);
    uint32_t cCyan    = packColor( 50, 220, 220);

    // Front face (Red)
    vertices[0]  = {{-1.0f, -1.0f,  1.0f}, qFront, cRed};
    vertices[1]  = {{ 1.0f, -1.0f,  1.0f}, qFront, cRed};
    vertices[2]  = {{ 1.0f,  1.0f,  1.0f}, qFront, cRed};
    vertices[3]  = {{-1.0f,  1.0f,  1.0f}, qFront, cRed};

    // Back face (Green)
    vertices[4]  = {{-1.0f, -1.0f, -1.0f}, qBack, cGreen};
    vertices[5]  = {{-1.0f,  1.0f, -1.0f}, qBack, cGreen};
    vertices[6]  = {{ 1.0f,  1.0f, -1.0f}, qBack, cGreen};
    vertices[7]  = {{ 1.0f, -1.0f, -1.0f}, qBack, cGreen};

    // Left face (Blue)
    vertices[8]  = {{-1.0f, -1.0f, -1.0f}, qLeft, cBlue};
    vertices[9]  = {{-1.0f, -1.0f,  1.0f}, qLeft, cBlue};
    vertices[10] = {{-1.0f,  1.0f,  1.0f}, qLeft, cBlue};
    vertices[11] = {{-1.0f,  1.0f, -1.0f}, qLeft, cBlue};

    // Right face (Yellow)
    vertices[12] = {{ 1.0f, -1.0f, -1.0f}, qRight, cYellow};
    vertices[13] = {{ 1.0f,  1.0f, -1.0f}, qRight, cYellow};
    vertices[14] = {{ 1.0f,  1.0f,  1.0f}, qRight, cYellow};
    vertices[15] = {{ 1.0f, -1.0f,  1.0f}, qRight, cYellow};

    // Top face (Magenta)
    vertices[16] = {{-1.0f,  1.0f, -1.0f}, qTop, cMagenta};
    vertices[17] = {{-1.0f,  1.0f,  1.0f}, qTop, cMagenta};
    vertices[18] = {{ 1.0f,  1.0f,  1.0f}, qTop, cMagenta};
    vertices[19] = {{ 1.0f,  1.0f, -1.0f}, qTop, cMagenta};

    // Bottom face (Cyan)
    vertices[20] = {{-1.0f, -1.0f, -1.0f}, qBottom, cCyan};
    vertices[21] = {{ 1.0f, -1.0f, -1.0f}, qBottom, cCyan};
    vertices[22] = {{ 1.0f, -1.0f,  1.0f}, qBottom, cCyan};
    vertices[23] = {{-1.0f, -1.0f,  1.0f}, qBottom, cCyan};

    // Generate indices (6 faces, 2 triangles per face)
    uint32_t idx = 0;
    for (uint32_t face = 0; face < 6; ++face) {
        uint16_t vStart = uint16_t(face * 4);
        indices[idx++] = vStart;
        indices[idx++] = uint16_t(vStart + 1);
        indices[idx++] = uint16_t(vStart + 2);
        indices[idx++] = vStart;
        indices[idx++] = uint16_t(vStart + 2);
        indices[idx++] = uint16_t(vStart + 3);
    }
}

} // namespace

int main() {
    std::printf("Hydragon Filament Cube Demo - starting (backend=Vulkan)\n");
    std::fflush(stdout);

    hydragon::HydragonCore core;
    if (!core.init()) {
        std::printf("HydragonCore::init failed\n");
        return 1;
    }

    HWND window = createWindow();
    if (window == nullptr) {
        std::printf("Failed to create window\n");
        core.shutdown();
        return 1;
    }
    ShowWindow(window, SW_SHOW);

    core.attachSurface((void*)window, kWidth, kHeight);

    // Get exposed Filament structures from the generic core
    filament::Engine* engine = core.getEngine();
    filament::Scene* scene = core.getScene();

    // Verification test of NodeGraph and SceneGraph hierarchy
    std::printf("[demo] Testing NodeGraph and SceneGraph hierarchy...\n");
    auto rootScene = std::make_shared<hd::Scene>("RootScene", engine);
    rootScene->loadFromFile("dummy_scene.usd");
    
    if (!rootScene->getChildren().empty()) {
        auto child = rootScene->getChildren()[0];
        std::printf("[demo] Created Node: %s (USD Path: %s, USD Type: %s)\n", 
                    child->getName().c_str(), 
                    child->getUsdPrim().GetPath().c_str(),
                    child->getUsdPrim().GetTypeName().c_str());
        
        // Test property synchronization
        child->setProperty<float>("roughness", 0.35f);
        std::printf("[demo] Property 'roughness' directly synced to USD: %.2f\n", 
                    child->getProperty<float>("roughness"));
    }
    std::fflush(stdout);

    // 1. Load the compiled lit material filamat package
    auto matBuffer = loadBinaryFile("lit_color.filamat");
    if (matBuffer.empty()) {
        std::printf("[demo] FAILED to load lit_color.filamat\n");
        std::fflush(stdout);
        core.shutdown();
        DestroyWindow(window);
        return 1;
    }

    filament::Material* material = filament::Material::Builder()
        .package(matBuffer.data(), matBuffer.size())
        .build(*engine);
    if (material == nullptr) {
        std::printf("[demo] FAILED to build material\n");
        std::fflush(stdout);
        core.shutdown();
        DestroyWindow(window);
        return 1;
    }

    // 2. Generate Cube vertices and indices
    Vertex cubeVertices[24];
    uint16_t cubeIndices[36];
    createCube(cubeVertices, cubeIndices);

    // 3. Allocate and set Vertex Buffer
    filament::VertexBuffer* vb = filament::VertexBuffer::Builder()
        .vertexCount(24)
        .bufferCount(1)
        .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT3, offsetof(Vertex, position), sizeof(Vertex))
        .attribute(filament::VertexAttribute::TANGENTS, 0, filament::VertexBuffer::AttributeType::FLOAT4, offsetof(Vertex, tangents), sizeof(Vertex))
        .attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4, offsetof(Vertex, color), sizeof(Vertex))
        .normalized(filament::VertexAttribute::COLOR)
        .build(*engine);
    vb->setBufferAt(*engine, 0,
        filament::VertexBuffer::BufferDescriptor(cubeVertices, sizeof(cubeVertices), nullptr));

    // 4. Allocate and set Index Buffer
    filament::IndexBuffer* ib = filament::IndexBuffer::Builder()
        .indexCount(36)
        .bufferType(filament::IndexBuffer::IndexType::USHORT)
        .build(*engine);
    ib->setBuffer(*engine,
        filament::IndexBuffer::BufferDescriptor(cubeIndices, sizeof(cubeIndices), nullptr));

    // 5. Create Renderable Entity (Cube)
    utils::Entity cube = utils::EntityManager::get().create();
    filament::RenderableManager::Builder(1)
        .boundingBox({{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}})
        .material(0, material->getDefaultInstance())
        .geometry(0, filament::RenderableManager::PrimitiveType::TRIANGLES, vb, ib, 0, 36)
        .culling(false)
        .castShadows(false)
        .receiveShadows(false)
        .build(*engine, cube);

    scene->addEntity(cube);

    // 6. Create basic Directional Light
    utils::Entity light = utils::EntityManager::get().create();
    filament::LightManager::Builder(filament::LightManager::Type::DIRECTIONAL)
        .color(filament::Color::toLinear(filament::RgbType::sRGB, {0.98f, 0.92f, 0.89f}))
        .intensity(100000.0f) // Lux
        .direction({-0.5f, -1.0f, -0.5f})
        .castShadows(false)
        .build(*engine, light);
    scene->addEntity(light);

    // 7. Create basic Skybox background
    filament::Skybox* skybox = filament::Skybox::Builder().color({0.1f, 0.1f, 0.2f, 1.0f}).build(*engine);
    scene->setSkybox(skybox);

    // 8. Initialize Nodal UI Subsystem
    auto uiManager = std::make_shared<hd::UIManager>("UIManager", engine);
    uiManager->initialize(window, kWidth, kHeight);
    core.registerView(uiManager->getUIView());

    auto uiPanel = std::make_shared<hd::UIPanel>("ConfigPanel", "Hydragon Config Panel");
    uiManager->addChild(uiPanel);

    auto uiText = std::make_shared<hd::UIText>("RotationSpeedText", "Cube Rotation Settings");
    uiPanel->addChild(uiText);

    auto speedSlider = std::make_shared<hd::UISlider>("SpeedSlider", "Speed Multiplier", 1.0f, 0.0f, 5.0f);
    uiPanel->addChild(speedSlider);

    auto colorButton = std::make_shared<hd::UIButton>("ColorButton", "Toggle Skybox Color");
    uiPanel->addChild(colorButton);

    std::printf("Render loop running. Press ESC or close to quit.\n");
    std::fflush(stdout);

    // Time tracking for rotation
    LARGE_INTEGER freq, start;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    LARGE_INTEGER lastTime = start;

    MSG msg = {};
    while (gRunning) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            if (msg.message == WM_QUIT) {
                gRunning = false;
            }
        }
        if (!gRunning) break;

        // Apply rotation to the cube and calculate deltaTime
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        const float t = float(double(now.QuadPart - start.QuadPart) / double(freq.QuadPart));
        const float deltaTime = float(double(now.QuadPart - lastTime.QuadPart) / double(freq.QuadPart));
        lastTime = now;

        // Start new UI frame
        uiManager->newFrame(deltaTime);

        // Read slider value to apply rotation speed
        float speedMultiplier = speedSlider->getProperty<float>("Value", 1.0f);

        auto& tcm = engine->getTransformManager();
        const auto inst = tcm.getInstance(cube);
        if (inst) {
            const mat4f model =
                mat4f::rotation(t * 0.9f * speedMultiplier, float3{0.0f, 1.0f, 0.0f}) *
                mat4f::rotation(t * 0.6f * speedMultiplier, float3{1.0f, 0.0f, 0.0f});
            tcm.setTransform(inst, model);
        }

        // Handle color toggle button click
        static bool alternateColor = false;
        if (colorButton->getProperty<bool>("Clicked", false)) {
            alternateColor = !alternateColor;
            auto* oldSkybox = skybox;
            if (alternateColor) {
                skybox = filament::Skybox::Builder().color({0.3f, 0.1f, 0.1f, 1.0f}).build(*engine);
            } else {
                skybox = filament::Skybox::Builder().color({0.1f, 0.1f, 0.2f, 1.0f}).build(*engine);
            }
            scene->setSkybox(skybox);
            engine->destroy(oldSkybox);
        }

        // Update the dynamic text block
        char speedStr[64];
        sprintf_s(speedStr, sizeof(speedStr), "Current Speed: %.2fx", speedMultiplier);
        uiText->setProperty<std::string>("Text", speedStr);

        // Process and draw the ImGui drawlists
        uiManager->render();

        core.tick();
    }

    std::printf("Shutting down.\n");
    std::fflush(stdout);

    // Cleanup resources in reverse order
    engine->destroy(light);
    engine->destroy(cube);
    engine->destroy(material);
    engine->destroy(ib);
    engine->destroy(vb);
    engine->destroy(skybox);

    uiManager->shutdown();
    core.shutdown();
    DestroyWindow(window);
    return 0;
}
