//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DescriptorStore.h"
#include "Log.h"
#include "Print.h"
#include "app/FirstInit.h"
#include "app/MainWindow.h"
#include "app/Settings.h"
#include "di/Getter.h"

namespace vul {
  class InstanceInit
  // BEAN instanceInit vul::DescriptorStore app::Settings vul::Print vul::Log app::FirstInit app::MainWindow
  {
    di::Getter<DescriptorStore> &descriptorStore_;
    di::Getter<app::Settings>   &setting_;
    di::Getter<Print>           &print_;
    di::Getter<Log>             &log_;
    di::Getter<app::FirstInit>  &firstInit_;
    di::Getter<app::MainWindow> &mainWindow_;

  public:
    explicit InstanceInit(di::Getter<DescriptorStore> &descriptorStore,
                          di::Getter<app::Settings>   &setting,
                          di::Getter<Print>           &print,
                          di::Getter<Log>             &log,
                          di::Getter<app::FirstInit>  &firstInit,
                          di::Getter<app::MainWindow> &mainWindow);

    void initTopObjects() const;

  private:
    void initVkInstance() const;
    void initVkMessenger() const;
    void selectVkPhysicalDevice() const;
    void createVkSdkSurface() const;
  };
} // namespace vul
