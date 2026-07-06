// FilamentCubeDemo.cpp
//
// Minimal Win32 front-end that drives the headless HydragonCore.
// Serves as a skeleton to rebuild the demo incrementally.

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
        0, wc.lpszClassName, L"Hydragon - Filament Demo Skeleton",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, instance, nullptr);
}

} // namespace

int main() {
    std::printf("Hydragon Filament Demo Skeleton - starting (backend=Vulkan)\n");
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

    std::printf("Render loop running. Press ESC or close to quit.\n");
    std::fflush(stdout);

    MSG msg = {};
    while (gRunning) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (!gRunning) break;

        core.renderFrame();
    }

    std::printf("Shutting down.\n");
    core.shutdown();
    DestroyWindow(window);
    return 0;
}
