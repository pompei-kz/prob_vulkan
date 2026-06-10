//
// Created by pompei on 2026-06-10.
//

#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

namespace vul::pipeline {

  class Pipeline
  {
  protected:
    VkDevice vkDevice_;

    explicit Pipeline(const VkDevice vkDevice)
        : vkDevice_(vkDevice)
    {}

  public:
    ~Pipeline() = default;

    void create();
  };

} // namespace vul::pipeline
