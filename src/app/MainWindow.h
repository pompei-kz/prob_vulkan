//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "FirstInit.h"
#include "cmd/Cmd.h"
#include "di/Getter.h"
#include "vul/TopStore.h"

#include <SDL3/SDL_video.h>
import SyncQueue;

namespace app {
  class MainWindow
  /// BEAN mainWindow
  /// DEPS app::FirstInit vul::TopStore
  {
    SDL_Window *window_ = nullptr;

    di::Getter<FirstInit>       &firstInit_;
    di::Getter<vul::TopStore>   &topStore_;
    util::SyncQueue<cmd::CmdPtr> cmdQueue_;

    std::function<void(cmd::CmdPtr)> cmdExecutor_;

  public:
    explicit MainWindow(di::Getter<FirstInit> &firstInit, di::Getter<vul::TopStore> &topStore)
        : firstInit_{firstInit}
        , topStore_{topStore}
    {}

    void create();

    void installCmdExecutor(std::function<void(cmd::CmdPtr)> cmdExecutor) { cmdExecutor_ = std::move(cmdExecutor); }

    void storeWindowPtr(SDL_Window *window);

    [[nodiscard]] SDL_Window *windowPtr() const { return window_; }

    void mainLoop(cmd::CmdPtr);

  private:
    void executeAllCmdQueue();
  };
} // namespace app
