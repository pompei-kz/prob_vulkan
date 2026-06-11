//
// Created by pompei on 2026-06-11.
//

#pragma once
#include "TopStore.h"
#include "di/Getter.h"
#include "util/Log.h"
#include "util/util.h"

namespace vul {

  class ImageStore_Worker
  /// BEAN imageStore_Worker
  /// DEPS vul::TopStore
  {
    di::Getter<TopStore> &topStore_;

  public:
    explicit ImageStore_Worker(di::Getter<TopStore> &topStore)
        : topStore_(topStore)
    {}

    void createDepthImage() const;
  };

  uint32_t findMemoryType(std::string_view            placeId,
                          const VkPhysicalDevice      physicalDevice,
                          const uint32_t              typeFilter,
                          const VkMemoryPropertyFlags properties);

} // namespace vul
