//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline.h"
#include "util/Log.h"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace vul::pipeline {

  class Pipeline_ShapeGroup : public Pipeline
  {
    VkCommandPool                vkCommandPool_ = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers_;

    VkBuffer cameraUboBuffer_ = VK_NULL_HANDLE;

  public:
    explicit Pipeline_ShapeGroup(const VkDevice vkDevice)
        : Pipeline(vkDevice)
    {}

    ~Pipeline_ShapeGroup() override { resetVkCommandPool(VK_NULL_HANDLE); }

    void resetVkCommandPool(const VkCommandPool vkCommandPool)
    {
      if (vkCommandPool_) {
        resetVkCommandBuffers(std::vector<VkCommandBuffer>());
        resetCameraUboBuffer(VK_NULL_HANDLE);

        vkDestroyCommandPool(device_, vkCommandPool_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("rFW7XX7zgk", "vkDestroyCommandPool({})", static_cast<void *>(vkCommandPool_));
        }
        vkCommandPool_ = VK_NULL_HANDLE;
      }

      vkCommandPool_ = vkCommandPool;
    }

    void resetVkCommandBuffers(const std::vector<VkCommandBuffer> &commandBuffers)
    {
      if (commandBuffers_.size() > 0) {
        vkFreeCommandBuffers(device_, vkCommandPool_, commandBuffers_.size(), commandBuffers_.data());

        if (util::Log::get()->hasVerbose()) {
          std::string str{};
          for (const auto commandBuffer : commandBuffers_) {
            if (str.length() > 0) str += ", ";
            str += std::format("{}", static_cast<void *>(commandBuffer));
          }
          util::Log::get()->verbose("fb2tPIM9l2", "vkFreeCommandBuffers({})", str);
        }
      }

      commandBuffers_ = commandBuffers;
    }

    [[nodiscard]] VkCommandPool vkCommandPool() const { return vkCommandPool_; }

    void resetCameraUboBuffer(const VkBuffer buffer)
    {
      if (cameraUboBuffer_) {
        vkDestroyBuffer(device_, cameraUboBuffer_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("FV3XSc6XFA", "Camera UBO: vkDestroyBuffer({})", static_cast<void *>(cameraUboBuffer_));
        }
        cameraUboBuffer_ = VK_NULL_HANDLE;
      }
      cameraUboBuffer_ = buffer;
    }

    [[nodiscard]] VkBuffer cameraUboBuffer() const { return cameraUboBuffer_; }
  };

} // namespace vul::pipeline
