//
// Created by pompei on 2026-06-10.
//

#include "Application.h"

namespace app {
  void Application::run() const
  {
    mainWindow_->create();
    initInstance_->initialize();

    mainWindow_->installCmdExecutor([this](const cmd::CmdPtr &cmdPtr) { executeCmd_->execute(cmdPtr); });

    const cmd::CmdPtr startCmd = backStart_->start();

    //
    //
    //
    mainWindow_->mainLoop(startCmd);
    //
    //
    //

    handleStore_->destroy();
    firstInit_->destroy();
  }
} // namespace app
