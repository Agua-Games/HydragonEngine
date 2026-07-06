// FilamentCubeDemo.cpp
//
// Thin Win32 front-end that drives the headless RenderCore (see HydragonCore.h).
//
// Responsibilities kept here (and ONLY here): create an OS window, pump the
// message loop, and hand the native handle to the core. All rendering/Filament
// logic lives in RenderCore, so the same core can run headless or behind a
// different front-end (editor, game, batch tool).
//
// This whole Demo/ folder is an isolated, removable sandbox: delete Demo/ and
// FilamentDemo.vcxproj to remove it without touching the engine entry point.

#include <cstdint>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "../HydragonCore.h"

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

} // namespace

int main() {
    std::printf("Hydragon Filament Cube Demo - starting (backend=Vulkan)\n");
    std::fflush(stdout);

    hydragon::RenderCore core;
    if (!core.init()) {
        std::printf("RenderCore::init failed\n");
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
    core.loadDemoScene();

    std::printf("Render loop running. Press ESC or close to quit.\n");
    std::fflush(stdout);

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

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        const float t = float(double(now.QuadPart - start.QuadPart) / double(freq.QuadPart));
        core.renderFrame(t);
    }

    std::printf("Shutting down.\n");
    core.shutdown();
    DestroyWindow(window);
    return 0;
}
