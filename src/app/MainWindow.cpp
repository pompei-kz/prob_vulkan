//
// Created by pompei on 2026-06-09.
//

#include "MainWindow.h"

namespace {
  constexpr int WINDOW_WIDTH  = 800;
  constexpr int WINDOW_HEIGHT = 600;
} // namespace

namespace app {

  MainWindow::MainWindow(di::Getter<FirstInit> &firstInit)
      : firstInit_(firstInit)
  {}

  void MainWindow::create()
  {
    firstInit_->init();

    SDL_Window * window = SDL_CreateWindow("New Vulkan Architecture", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
      throw std::runtime_error(std::string("aT5sJ9qBvE :: ") + SDL_GetError());
    }

    storeWindowPtr(window);
  }

} // namespace app
