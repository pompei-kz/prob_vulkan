//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline.h"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace vul::pipeline {

  class Pipeline_ShapeGroup : public Pipeline
  {
    VkCommandPool                vkCommandPool_ = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers_;

  public:
    explicit Pipeline_ShapeGroup(const VkDevice vkDevice)
        : Pipeline(vkDevice)
    {}

    ~Pipeline_ShapeGroup()
    {
      storeVkCommandPool(VK_NULL_HANDLE);
    }

    void storeVkCommandPool(const VkCommandPool vkCommandPool)
    {
      if (vkCommandPool_ == vkCommandPool) return;

      if (vkCommandPool_) {
        storeVkCommandBuffers(std::vector<VkCommandBuffer>());
        vkDestroyCommandPool(vkDevice_, vkCommandPool_, nullptr);
        vkCommandPool_ = VK_NULL_HANDLE;
      }

      vkCommandPool_ = vkCommandPool;
    }

    void storeVkCommandBuffers(const std::vector<VkCommandBuffer> &commandBuffers)
    {
      if (commandBuffers_ == commandBuffers) return;

      vkFreeCommandBuffers(vkDevice_, vkCommandPool_, commandBuffers_.size(), commandBuffers_.data());

      commandBuffers_ = commandBuffers;
    }

    [[nodiscard]] VkCommandPool vkCommandPool() const
    {
      return vkCommandPool_;
    }
  };

} // namespace vul::pipeline
