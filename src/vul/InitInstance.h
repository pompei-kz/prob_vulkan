//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DescriptorStore.h"
#include "Log.h"
#include "Print.h"
#include "SelectPhysicalDevice.h"
#include "app/FirstInit.h"
#include "app/MainWindow.h"
#include "app/Settings.h"
#include "di/Getter.h"

namespace vul {
  class InitInstance
  /// BEAN instanceInit vul::DescriptorStore app::Settings vul::Print vul::Log app::FirstInit app::MainWindow vul::SelectPhysicalDevice
  {
    di::Getter<DescriptorStore>      &descriptorStore_;
    di::Getter<app::Settings>        &setting_;
    di::Getter<Print>                &print_;
    di::Getter<Log>                  &log_;
    di::Getter<app::FirstInit>       &firstInit_;
    di::Getter<app::MainWindow>      &mainWindow_;
    di::Getter<SelectPhysicalDevice> &selectPhysicalDevice_;

  public:
    explicit InitInstance(di::Getter<DescriptorStore>      &descriptorStore,
                          di::Getter<app::Settings>        &setting,
                          di::Getter<Print>                &print,
                          di::Getter<Log>                  &log,
                          di::Getter<app::FirstInit>       &firstInit,
                          di::Getter<app::MainWindow>      &mainWindow,
                          di::Getter<SelectPhysicalDevice> &selectPhysicalDevice)
        : descriptorStore_(descriptorStore)
        , setting_(setting)
        , print_(print)
        , log_(log)
        , firstInit_(firstInit)
        , mainWindow_(mainWindow)
        , selectPhysicalDevice_(selectPhysicalDevice)
    {}

    void initTopObjects() const;

  private:
    void createVkInstance() const;
    void initVkMessenger() const;
    void createVkSdkSurface() const;
    void createDevice() const;
    void createSwapChain()const;
  };
} // namespace vul
