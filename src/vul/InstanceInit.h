//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DescriptorStore.h"
#include "Log.h"
#include "Print.h"
#include "app/Settings.h"
#include "di/Getter.h"

namespace vul {
  class InstanceInit
  // BEAN instanceInit vul::DescriptorStore app::Settings vul::Print vul::Log
  {
    di::Getter<DescriptorStore> &descriptorStore_;
    di::Getter<app::Settings>   &setting_;
    di::Getter<Print>           &print_;
    di::Getter<Log>             &log_;

  public:
    explicit InstanceInit(di::Getter<DescriptorStore> &descriptorStore,
                          di::Getter<app::Settings>   &setting,
                          di::Getter<Print>           &print,
                          di::Getter<Log>             &log);

    void initTopObjects() const;

  private:
    void initSDL() const;
    void initVkInstance() const;
    void initVkMessenger() const;
    void initVkPhysicalDevice() const;
  };
} // namespace vul
