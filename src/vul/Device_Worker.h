//
// Created by pompei on 2026-06-11.
//

#pragma once
#include "Device.h"
#include "HandleStore.h"
#include "di/Getter.h"
#include "model/QueueFamilyIndices.h"
#include "util/util.h"

#include <memory>
#include <set>
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace vul {

  class Device_Worker
  /// BEAN device_Worker vul::HandleStore
  {
    di::Getter<HandleStore> &handleStore_;

  public:
    explicit Device_Worker(di::Getter<HandleStore> &handleStore)
        : handleStore_(handleStore)
    {}

    void create() const;

  private:
    model::QueueFamilyIndices selectRequiredQueueFamilyIndices() const;
  };

} // namespace vul
