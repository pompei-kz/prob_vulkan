//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "Device_Worker.h"
#include "Print.h"
#include "SelectPhysicalDevice.h"
#include "SwapChain_Worker.h"
#include "TopStore.h"
#include "app/FirstInit.h"
#include "app/MainWindow.h"
#include "app/Settings.h"
#include "di/Getter.h"

namespace vul {
  class InitInstance
  /// BEAN initInstance
  /// DEPS vul::TopStore app::Settings vul::Print app::FirstInit app::MainWindow vul::SelectPhysicalDevice
  /// DEPS vul::SwapChain_Worker vul::Device_Worker
  {
    di::Getter<TopStore>             &topStore_;
    di::Getter<app::Settings>        &setting_;
    di::Getter<Print>                &print_;
    di::Getter<app::FirstInit>       &firstInit_;
    di::Getter<app::MainWindow>      &mainWindow_;
    di::Getter<SelectPhysicalDevice> &selectPhysicalDevice_;
    di::Getter<SwapChain_Worker>     &swapChain_Worker_;
    di::Getter<Device_Worker>        &device_Worker_;

  public:
    explicit InitInstance(di::Getter<TopStore>             &topStore,
                          di::Getter<app::Settings>        &setting,
                          di::Getter<Print>                &print,
                          di::Getter<app::FirstInit>       &firstInit,
                          di::Getter<app::MainWindow>      &mainWindow,
                          di::Getter<SelectPhysicalDevice> &selectPhysicalDevice,
                          di::Getter<SwapChain_Worker>     &swapChain_Worker,
                          di::Getter<Device_Worker>        &device_Worker)
        : topStore_(topStore)
        , setting_(setting)
        , print_(print)
        , firstInit_(firstInit)
        , mainWindow_(mainWindow)
        , selectPhysicalDevice_(selectPhysicalDevice)
        , swapChain_Worker_(swapChain_Worker)
        , device_Worker_(device_Worker)
    {}

    void initialize() const;

  private:
    void createVkInstance() const;
    void initVkMessenger() const;
    void createVkSdkSurface() const;
  };
} // namespace vul
