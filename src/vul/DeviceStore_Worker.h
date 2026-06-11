//
// Created by pompei on 2026-06-11.
//

#pragma once
#include "DeviceStore.h"
#include "TopStore.h"
#include "di/Getter.h"
#include "model/QueueFamilyIndices.h"
#include "util/util.h"

#include <memory>
#include <set>
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace vul {

  class DeviceStore_Worker
  /// BEAN deviceStore_Worker
  /// DEPS vul::TopStore
  {
    di::Getter<TopStore> &topStore_;

  public:
    explicit DeviceStore_Worker(di::Getter<TopStore> &topStore)
        : topStore_(topStore)
    {}

    void create() const;

  private:
    model::QueueFamilyIndices selectRequiredQueueFamilyIndices() const;
  };

} // namespace vul
