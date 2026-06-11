//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DeviceStore_Worker.h"
#include "PhysicalDevice.h"
#include "Print.h"
#include "SwapChainStore_Worker.h"
#include "TopStore.h"
#include "app/FirstInit.h"
#include "app/MainWindow.h"
#include "app/Settings.h"
#include "di/Getter.h"

namespace vul {
  class TopStore_Worker
  /// BEAN topStore_Worker
  /// DEPS vul::TopStore app::Settings vul::Print app::FirstInit app::MainWindow vul::PhysicalDevice
  /// DEPS vul::SwapChainStore_Worker vul::DeviceStore_Worker
  {
    di::Getter<TopStore>               &topStore_;
    di::Getter<app::Settings>          &setting_;
    di::Getter<Print>                  &print_;
    di::Getter<app::FirstInit>         &firstInit_;
    di::Getter<app::MainWindow>        &mainWindow_;
    di::Getter<PhysicalDevice>         &physicalDevice_;
    di::Getter<SwapChainStore_Worker>  &swapChainStore_Worker_;
    di::Getter<DeviceStore_Worker>     &deviceStore_Worker_;

  public:
    explicit TopStore_Worker(di::Getter<TopStore>               &topStore,
                             di::Getter<app::Settings>          &setting,
                             di::Getter<Print>                  &print,
                             di::Getter<app::FirstInit>         &firstInit,
                             di::Getter<app::MainWindow>        &mainWindow,
                             di::Getter<PhysicalDevice>         &physicalDevice,
                             di::Getter<SwapChainStore_Worker>  &swapChainStore_Worker,
                             di::Getter<DeviceStore_Worker>     &deviceStore_Worker)
        : topStore_(topStore)
        , setting_(setting)
        , print_(print)
        , firstInit_(firstInit)
        , mainWindow_(mainWindow)
        , physicalDevice_(physicalDevice)
        , swapChainStore_Worker_(swapChainStore_Worker)
        , deviceStore_Worker_(deviceStore_Worker)
    {}

    void initialize() const;

  private:
    void createVkInstance() const;
    void initVkMessenger() const;
    void createVkSdkSurface() const;
  };
} // namespace vul
