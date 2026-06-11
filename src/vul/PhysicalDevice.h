//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "../util/Log.h"
#include "Print.h"
#include "TopStore.h"
#include "di/Getter.h"
#include "model/QueueFamilyIndices.h"

namespace vul {

  class PhysicalDevice
  /// BEAN physicalDevice
  /// DEPS vul::TopStore app::Settings vul::Print
  {
    di::Getter<TopStore>      &topStore_;
    di::Getter<app::Settings> &setting_;
    di::Getter<Print>         &print_;

  public:
    explicit PhysicalDevice(di::Getter<TopStore> &topStore, di::Getter<app::Settings> &setting, di::Getter<Print> &print)
        : topStore_(topStore)
        , setting_(setting)
        , print_(print)
    {}

    void select() const;

  private:
    uint64_t resetDevice(const VkPhysicalDevice vkPhysicalDevice) const;

    bool isDeviceSuitable(const VkPhysicalDevice vkPhysicalDevice) const;

    static bool checkDeviceExtensionSupport(const VkPhysicalDevice vkPhysicalDevice);
  };

} // namespace vul
