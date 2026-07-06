#include <cstdio>
#include "HydragonCore.h"

int main()
{
    std::printf("Hydragon Next - core online (Phase 0 toolchain OK)\n");

#if defined(HYDRAGON_USE_FILAMENT)
    hydragon::HydragonCore core;
    if (core.init()) {
        std::printf("Filament integration: ENABLED and HydragonCore initialized successfully (backend=Vulkan)\n");
        core.shutdown();
    } else {
        std::printf("Filament integration: ENABLED but HydragonCore initialization FAILED\n");
    }
#else
    std::printf("Filament integration: pending (run scripts/setup.ps1, set UseFilament=true)\n");
#endif

    return 0;
}
