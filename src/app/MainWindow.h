//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "FirstInit.h"
#include "cmd/Cmd.h"
#include "di/Getter.h"
#include "vul/DescriptorStore.h"

#include <SDL3/SDL_video.h>
import SyncQueue;

namespace app {
  class MainWindow
  /// BEAN mainWindow app::FirstInit vul::DescriptorStore
  {
    SDL_Window *window_ = nullptr;

    di::Getter<FirstInit>            &firstInit_;
    di::Getter<vul::DescriptorStore> &descriptorStore_;
    util::SyncQueue<cmd::CmdPtr>      cmdQueue_;

    std::function<void(cmd::CmdPtr)> cmdExecutor_;

  public:
    explicit MainWindow(di::Getter<FirstInit> &firstInit, di::Getter<vul::DescriptorStore> &descriptorStore)
        : firstInit_{firstInit}
        , descriptorStore_{descriptorStore}
    {}

    void create();

    void installCmdExecutor(std::function<void(cmd::CmdPtr)> cmdExecutor)
    {
      cmdExecutor_ = std::move(cmdExecutor);
    }

    void storeWindowPtr(SDL_Window *window);

    [[nodiscard]] SDL_Window *windowPtr() const
    {
      return window_;
    }

    void mainLoop(cmd::CmdPtr);

  private:
    void executeAllCmdQueue();
  };
} // namespace app
