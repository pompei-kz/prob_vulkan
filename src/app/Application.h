//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "FirstInit.h"
#include "MainWindow.h"
#include "back/BackStart.h"
#include "cmd/ExecuteCmd.h"
#include "vul/TopStore_Worker.h"

namespace app {

  class Application
  /// BEAN application
  /// DEPS app::MainWindow vul::TopStore_Worker back::BackStart vul::TopStore app::FirstInit cmd::ExecuteCmd
  {

    di::Getter<MainWindow>          &mainWindow_;
    di::Getter<vul::TopStore_Worker> &topStore_Worker_;
    di::Getter<back::BackStart>     &backStart_;
    di::Getter<vul::TopStore>       &topStore_;
    di::Getter<FirstInit>           &firstInit_;
    di::Getter<cmd::ExecuteCmd>     &executeCmd_;

  public:
    Application(di::Getter<MainWindow>          &mainWindow,
                di::Getter<vul::TopStore_Worker> &topStore_Worker,
                di::Getter<back::BackStart>     &backStart,
                di::Getter<vul::TopStore>       &topStore,
                di::Getter<FirstInit>           &firstInit,
                di::Getter<cmd::ExecuteCmd>     &executeCmd)
        : mainWindow_(mainWindow)
        , topStore_Worker_(topStore_Worker)
        , backStart_(backStart)
        , topStore_(topStore)
        , firstInit_(firstInit)
        , executeCmd_(executeCmd)
    {}

    void run() const;
  };

} // namespace app
