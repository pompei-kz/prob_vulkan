//
// Created by pompei on 2026-06-09.
//

#include "MainWindow.h"

#include "vul/Device.h"

#include <SDL3/SDL_events.h>

namespace {
  constexpr int WINDOW_WIDTH  = 800;
  constexpr int WINDOW_HEIGHT = 600;
} // namespace

namespace app {

  void MainWindow::create()
  {
    firstInit_->init();

    SDL_Window *window = SDL_CreateWindow("New Vulkan Architecture", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
      throw std::runtime_error(std::string("aT5sJ9qBvE :: ") + SDL_GetError());
    }

    storeWindowPtr(window);
  }

  void MainWindow::storeWindowPtr(SDL_Window *window)
  {
    if (window_) {
      SDL_DestroyWindow(window_);
      window_ = nullptr;
    }

    window_ = window;
  }

  void MainWindow::mainLoop(cmd::CmdPtr cmdPtr)
  {
    cmdQueue_.push_back(std::move(cmdPtr));

    bool working = true;
    while (working) {
      bool frameBufferResized = false;

      SDL_Event event{};
      while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_EVENT_QUIT) {
          working = false;
        } else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
          frameBufferResized = true;
        }
      }

      executeAllCmdQueue();
    }

    vkDeviceWaitIdle(topStore_->device()->handle());
  }

  void MainWindow::executeAllCmdQueue()
  {
    if (const auto exeFunc = cmdExecutor_; exeFunc) {
      for (;;) {
        std::optional<cmd::CmdPtr> cmdPtr = cmdQueue_.pop_front();
        if (!cmdPtr.has_value()) {
          return;
        }

        exeFunc(cmdPtr.value());
      }
    }
  }

} // namespace app
