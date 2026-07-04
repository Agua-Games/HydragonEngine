#include <cstdio>

// Hydragon "next" core — Phase 0 smoke test.
//
// Goal of this file right now: prove that the Visual Studio + vcpkg toolchain
// links and runs. Filament/USD are wired in later phases (see README).
//
// Phase 1 (Filament hello-cube) goes behind HYDRAGON_USE_FILAMENT, which is
// enabled by setting <UseFilament>true</UseFilament> once the prebuilt
// Filament SDK is present under ThirdParty/Filament (run scripts/setup.ps1).

#if defined(HYDRAGON_USE_FILAMENT)
// #include <filament/Engine.h>
// #include <filament/Renderer.h>
// #include <filament/Scene.h>
// #include <filament/View.h>
// TODO(Phase 1): create Engine, SwapChain, Renderer, a Camera and render a cube.
#endif

int main()
{
    std::printf("Hydragon Next - core online (Phase 0 toolchain OK)\n");

#if defined(HYDRAGON_USE_FILAMENT)
    std::printf("Filament integration: ENABLED\n");
#else
    std::printf("Filament integration: pending (run scripts/setup.ps1, set UseFilament=true)\n");
#endif

    return 0;
}
