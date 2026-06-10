//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DescriptorStore.h"
#include "Log.h"
#include "Print.h"
#include "di/Getter.h"
#include "model/QueueFamilyIndices.h"

namespace vul {

  class SelectPhysicalDevice
  /// BEAN initPhysicalDevice vul::DescriptorStore app::Settings vul::Print
  {
    di::Getter<DescriptorStore> &descriptorStore_;
    di::Getter<app::Settings>   &setting_;
    di::Getter<Print>           &print_;

  public:
    explicit SelectPhysicalDevice(di::Getter<DescriptorStore> &descriptorStore,
                                  di::Getter<app::Settings>   &setting,
                                  di::Getter<Print>           &print)
        : descriptorStore_(descriptorStore)
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
