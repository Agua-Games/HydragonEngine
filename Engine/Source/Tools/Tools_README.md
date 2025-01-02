
Why a separate /Tools folder (and not /DevTools)?

/Core/Tools/ is intended for the source code of tools that are not part of the engine, but are useful for development, and different than the tools in DevTools, they are intended to be available to users in general, not only devs, in the form of standalone applications - for instance a simpler editor for editing assets, or a simpler editor for editing the engine's configuration.

/Core/DevTools/ is intended for the source code of tools that are part of the engine, and are useful for development, and different than the tools in Tools, they are intended to be available to developers only, in the form of plugins - for instance a tool for visualizing the memory usage of the engine, or a tool for visualizing the rendering of the engine.
