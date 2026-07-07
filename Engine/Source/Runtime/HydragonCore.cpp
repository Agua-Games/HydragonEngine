// HydragonCore.cpp
//
// Implementation of the headless core - including data/scene, gameplay,
// physics, render, audio, streaming

#include "HydragonCore.h"

#include <cstdio>
#include <filament/Viewport.h>
#include <utils/EntityManager.h>

using namespace filament;

namespace hydragon {

bool HydragonCore::init() {
  std::printf("[core] before Engine::create\n");
  std::fflush(stdout);
  mEngine = Engine::create(backend::Backend::VULKAN);
  if (mEngine == nullptr) {
    std::printf("[core] Engine::create returned null\n");
    std::fflush(stdout);
    return false;
  }
  std::printf("[core] engine created\n");
  std::fflush(stdout);

  mRenderer = mEngine->createRenderer();
  mScene = mEngine->createScene();
  mView = mEngine->createView();

  mCameraEntity = utils::EntityManager::get().create();
  mCamera = mEngine->createCamera(mCameraEntity);

  mView->setScene(mScene);
  mView->setCamera(mCamera);
  std::printf("[core] renderer/scene/view/camera created\n");
  std::fflush(stdout);
  return true;
}

void HydragonCore::attachSurface(void *nativeWindow, uint32_t width,
                               uint32_t height) {
  mWidth = width;
  mHeight = height;

  std::printf("[core] before createSwapChain\n");
  std::fflush(stdout);
  mSwapChain = mEngine->createSwapChain(nativeWindow);
  std::printf("[core] swapchain created\n");
  std::fflush(stdout);

  mView->setViewport({0, 0, width, height});
  mCamera->setProjection(45.0, double(width) / double(height), 0.1, 100.0,
                         Camera::Fov::VERTICAL);
  mCamera->lookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
}

void HydragonCore::tick() {
  // In the future: update input, physics, scene graph delta sync, etc.
  renderFrame();
}

void HydragonCore::renderFrame() {
  if (mSwapChain == nullptr) {
    return;
  }

  if (mRenderer->beginFrame(mSwapChain)) {
    mRenderer->render(mView);
    mRenderer->endFrame();
  }
}

void HydragonCore::shutdown() {
  if (mEngine == nullptr) {
    return;
  }
  if (mView)
    mEngine->destroy(mView);
  if (mScene)
    mEngine->destroy(mScene);
  if (mRenderer)
    mEngine->destroy(mRenderer);
  if (mCamera)
    mEngine->destroyCameraComponent(mCameraEntity);
  if (mCameraEntity)
    utils::EntityManager::get().destroy(mCameraEntity);
  if (mSwapChain)
    mEngine->destroy(mSwapChain);
  Engine::destroy(&mEngine);
  mEngine = nullptr;
  std::printf("[core] shutdown complete\n");
  std::fflush(stdout);
}

} // namespace hydragon
