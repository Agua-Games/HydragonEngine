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

#include <filament/VertexBuffer.h>
#include <filament/IndexBuffer.h>
#include <filament/RenderableManager.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/TransformManager.h>
#include <filament/Skybox.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat4.h>
#include <utils/EntityManager.h>

using filament::math::float3;
using filament::math::mat4f;

namespace {

constexpr uint32_t kWidth = 1280;
constexpr uint32_t kHeight = 720;

bool gRunning = true;

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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
    uint32_t color;
};

// 8 cube corners, each with a distinct color (colors interpolate across faces).
const Vertex kVertices[8] = {
    {{-1.0f, -1.0f, -1.0f}, packColor(255,   0,   0)},  // red
    {{ 1.0f, -1.0f, -1.0f}, packColor(  0, 255,   0)},  // green
    {{ 1.0f,  1.0f, -1.0f}, packColor(  0,   0, 255)},  // blue
    {{-1.0f,  1.0f, -1.0f}, packColor(255, 255,   0)},  // yellow
    {{-1.0f, -1.0f,  1.0f}, packColor(255,   0, 255)},  // magenta
    {{ 1.0f, -1.0f,  1.0f}, packColor(  0, 255, 255)},  // cyan
    {{ 1.0f,  1.0f,  1.0f}, packColor(255, 255, 255)},  // white
    {{-1.0f,  1.0f,  1.0f}, packColor(255, 128,   0)},  // orange
};

// 12 triangles (36 indices), CCW winding.
const uint16_t kIndices[36] = {
    0, 1, 2,  0, 2, 3,   // back
    4, 6, 5,  4, 7, 6,   // front
    4, 5, 1,  4, 1, 0,   // bottom
    3, 2, 6,  3, 6, 7,   // top
    1, 5, 6,  1, 6, 2,   // right
    4, 0, 3,  4, 3, 7,   // left
};

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

    // 1. Load the compiled material filamat package
    auto matBuffer = loadBinaryFile("unlit_color.filamat");
    if (matBuffer.empty()) {
        std::printf("[demo] FAILED to load unlit_color.filamat\n");
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

    // 2. Allocate and set Vertex Buffer
    filament::VertexBuffer* vb = filament::VertexBuffer::Builder()
        .vertexCount(8)
        .bufferCount(1)
        .attribute(filament::VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT3, 0, sizeof(Vertex))
        .attribute(filament::VertexAttribute::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4, 12, sizeof(Vertex))
        .normalized(filament::VertexAttribute::COLOR)
        .build(*engine);
    vb->setBufferAt(*engine, 0,
        filament::VertexBuffer::BufferDescriptor(kVertices, sizeof(kVertices), nullptr));

    // 3. Allocate and set Index Buffer
    filament::IndexBuffer* ib = filament::IndexBuffer::Builder()
        .indexCount(36)
        .bufferType(filament::IndexBuffer::IndexType::USHORT)
        .build(*engine);
    ib->setBuffer(*engine,
        filament::IndexBuffer::BufferDescriptor(kIndices, sizeof(kIndices), nullptr));

    // 4. Create Renderable Entity (Cube)
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

    // 5. Create basic Skybox background
    filament::Skybox* skybox = filament::Skybox::Builder().color({0.1f, 0.1f, 0.2f, 1.0f}).build(*engine);
    scene->setSkybox(skybox);

    std::printf("Render loop running. Press ESC or close to quit.\n");
    std::fflush(stdout);

    // Time tracking for rotation
    LARGE_INTEGER freq, start;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    MSG msg = {};
    while (gRunning) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (!gRunning) break;

        // Apply rotation to the cube
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        const float t = float(double(now.QuadPart - start.QuadPart) / double(freq.QuadPart));

        auto& tcm = engine->getTransformManager();
        const auto inst = tcm.getInstance(cube);
        if (inst) {
            const mat4f model =
                mat4f::rotation(t * 0.9f, float3{0.0f, 1.0f, 0.0f}) *
                mat4f::rotation(t * 0.6f, float3{1.0f, 0.0f, 0.0f});
            tcm.setTransform(inst, model);
        }

        core.tick();
    }

    std::printf("Shutting down.\n");
    std::fflush(stdout);

    // Cleanup resources in reverse order
    engine->destroy(cube);
    engine->destroy(material);
    engine->destroy(ib);
    engine->destroy(vb);
    engine->destroy(skybox);

    core.shutdown();
    DestroyWindow(window);
    return 0;
}
