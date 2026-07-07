// HydragonCore.h
//
// Headless engine core (generic engine initialization for: data/scene,
// gameplay, physics, render, audio, streaming). Platform and windowing agnostic
// core.

#pragma once

#include <cstdint>

#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/View.h>

#include <utils/Entity.h>

namespace hydragon {

class HydragonCore {
public:
  // Create the Filament engine and the renderer-side objects (Scene, View,
  // Camera, Renderer).
  bool init();

  // Bind an OS surface so frames can be presented. Pass an opaque native window
  // handle.
  void attachSurface(void *nativeWindow, uint32_t width, uint32_t height);

  // Advance and render one frame.
  void renderFrame();

  // Tear everything down in reverse order of creation.
  void shutdown();

  bool hasSurface() const { return mSwapChain != nullptr; }

  filament::Engine* getEngine() const { return mEngine; }
  filament::Scene* getScene() const { return mScene; }
  filament::View* getView() const { return mView; }
  filament::Camera* getCamera() const { return mCamera; }

private:
  filament::Engine *mEngine = nullptr;
  filament::SwapChain *mSwapChain = nullptr;
  filament::Renderer *mRenderer = nullptr;
  filament::Scene *mScene = nullptr;
  filament::View *mView = nullptr;
  filament::Camera *mCamera = nullptr;
  utils::Entity mCameraEntity;

  uint32_t mWidth = 0;
  uint32_t mHeight = 0;
};

} // namespace hydragon
