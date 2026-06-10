//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "FirstInit.h"
#include "MainWindow.h"
#include "back/BackStart.h"
#include "cmd/ExecuteCmd.h"
#include "vul/InitInstance.h"

namespace app {

  class Application
  /// BEAN application app::MainWindow vul::InitInstance back::BackStart vul::HandleStore app::FirstInit cmd::ExecuteCmd
  {

    di::Getter<MainWindow>        &mainWindow_;
    di::Getter<vul::InitInstance> &initInstance_;
    di::Getter<back::BackStart>   &backStart_;
    di::Getter<vul::HandleStore>  &handleStore_;
    di::Getter<FirstInit>         &firstInit_;
    di::Getter<cmd::ExecuteCmd>   &executeCmd_;

  public:
    Application(di::Getter<MainWindow>        &mainWindow,
                di::Getter<vul::InitInstance> &initInstance,
                di::Getter<back::BackStart>   &backStart,
                di::Getter<vul::HandleStore>  &handleStore,
                di::Getter<FirstInit>         &firstInit,
                di::Getter<cmd::ExecuteCmd>   &executeCmd)
        : mainWindow_(mainWindow)
        , initInstance_(initInstance)
        , backStart_(backStart)
        , handleStore_(handleStore)
        , firstInit_(firstInit)
        , executeCmd_(executeCmd)
    {}

    void run() const;
  };

} // namespace app
