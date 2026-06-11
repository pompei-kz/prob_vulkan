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
    VkDevice device_;

    explicit Pipeline(const VkDevice device)
        : device_(device)
    {}

  public:
    virtual ~Pipeline() = default;
  };

} // namespace vul::pipeline
