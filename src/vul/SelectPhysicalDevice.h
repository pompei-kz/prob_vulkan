//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "../util/Log.h"
#include "HandleStore.h"
#include "Print.h"
#include "di/Getter.h"
#include "model/QueueFamilyIndices.h"

namespace vul {

  class SelectPhysicalDevice
  /// BEAN initPhysicalDevice
  /// DEPS vul::HandleStore app::Settings vul::Print
  {
    di::Getter<HandleStore>   &handleStore_;
    di::Getter<app::Settings> &setting_;
    di::Getter<Print>         &print_;

  public:
    explicit SelectPhysicalDevice(di::Getter<HandleStore> &handleStore, di::Getter<app::Settings> &setting, di::Getter<Print> &print)
        : handleStore_(handleStore)
        , setting_(setting)
        , print_(print)
    {}

    void select() const;

  private:
    uint64_t scoreDevice(const VkPhysicalDevice vkPhysicalDevice) const;

    bool isDeviceSuitable(const VkPhysicalDevice vkPhysicalDevice) const;

    static bool checkDeviceExtensionSupport(const VkPhysicalDevice vkPhysicalDevice);
  };

} // namespace vul
