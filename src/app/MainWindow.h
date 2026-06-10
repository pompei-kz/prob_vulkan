//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "FirstInit.h"
#include "di/Getter.h"
#include "vul/DescriptorStore.h"

#include <SDL3/SDL_video.h>

namespace app {
  class MainWindow
  /// BEAN mainWindow app::FirstInit vul::DescriptorStore
  {
    SDL_Window *window_ = nullptr;

    di::Getter<FirstInit>            &firstInit_;
    di::Getter<vul::DescriptorStore> &descriptorStore_;

  public:
    explicit MainWindow(di::Getter<FirstInit> &firstInit, di::Getter<vul::DescriptorStore> &descriptorStore)
        : firstInit_{firstInit}
        , descriptorStore_{descriptorStore}
    {}

    void create();

    void storeWindowPtr(SDL_Window *window);

    [[nodiscard]] SDL_Window *windowPtr() const
    {
      return window_;
    }

    void mainLoop();
  };
} // namespace app
