//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "FirstInit.h"
#include "di/Getter.h"

#include <SDL3/SDL_video.h>

namespace app {
  class MainWindow
  /// BEAN mainWindow app::FirstInit
  {
    SDL_Window *window_ = nullptr;

    di::Getter<FirstInit> &firstInit_;

  public:
    explicit MainWindow(di::Getter<FirstInit> &firstInit);

    void create();

    void storeWindowPtr(SDL_Window *window)
    {
      if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
      }

      window_ = window;
    }

    [[nodiscard]] SDL_Window *windowPtr() const
    {
      return window_;
    }
  };
} // namespace app
