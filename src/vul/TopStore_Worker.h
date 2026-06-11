
//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DeviceStore_Worker.h"
#include "ImageStore_Worker.h"
#include "PhysicalDevice_Worker.h"
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
  /// DEPS vul::TopStore app::Settings vul::Print app::FirstInit app::MainWindow vul::PhysicalDevice_Worker
  /// DEPS vul::SwapChainStore_Worker vul::DeviceStore_Worker vul::ImageStore_Worker
  {
    di::Getter<TopStore>              &topStore_;
    di::Getter<app::Settings>         &setting_;
    di::Getter<Print>                 &print_;
    di::Getter<app::FirstInit>        &firstInit_;
    di::Getter<app::MainWindow>       &mainWindow_;
    di::Getter<PhysicalDevice_Worker> &physicalDevice_Worker_;
    di::Getter<SwapChainStore_Worker> &swapChainStore_Worker_;
    di::Getter<DeviceStore_Worker>    &deviceStore_Worker_;
    di::Getter<ImageStore_Worker>     &imageStore_worker_;

  public:
    explicit TopStore_Worker(di::Getter<TopStore>              &topStore,
                             di::Getter<app::Settings>         &setting,
                             di::Getter<Print>                 &print,
                             di::Getter<app::FirstInit>        &firstInit,
                             di::Getter<app::MainWindow>       &mainWindow,
                             di::Getter<PhysicalDevice_Worker> &physicalDevice_Worker,
                             di::Getter<SwapChainStore_Worker> &swapChainStore_Worker,
                             di::Getter<DeviceStore_Worker>    &deviceStore_Worker,
                             di::Getter<ImageStore_Worker>     &imageStore_Worker)
        : topStore_(topStore)
        , setting_(setting)
        , print_(print)
        , firstInit_(firstInit)
        , mainWindow_(mainWindow)
        , physicalDevice_Worker_(physicalDevice_Worker)
        , swapChainStore_Worker_(swapChainStore_Worker)
        , deviceStore_Worker_(deviceStore_Worker)
        , imageStore_worker_(imageStore_Worker)
    {}

    void initialize() const;

  private:
    void createVkInstance() const;
    void initVkMessenger() const;
    void createVkSdkSurface() const;
  };
} // namespace vul
