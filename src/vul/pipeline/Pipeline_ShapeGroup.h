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

    VkDescriptorSetLayout cameraUboLayout_    = VK_NULL_HANDLE;
    VkBuffer              cameraUboBuffer_    = VK_NULL_HANDLE;
    VkDeviceMemory        cameraUboMemory_    = VK_NULL_HANDLE;
    void                 *cameraUboMemoryRef_ = nullptr;

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
        resetDescriptorSetLayout(VK_NULL_HANDLE);
        resetCameraUboMemory(VK_NULL_HANDLE);

        vkDestroyCommandPool(device_, vkCommandPool_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("rFW7XX7zgk", "vkDestroyCommandPool(), H={}", static_cast<void *>(vkCommandPool_));
        }
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
          util::Log::get()->verbose("fb2tPIM9l2", "vkFreeCommandBuffers(), H={}", str);
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
          util::Log::get()->verbose("FV3XSc6XFA", "Camera UBO: vkDestroyBuffer(), H={}", static_cast<void *>(cameraUboBuffer_));
        }
      }
      cameraUboBuffer_ = buffer;
    }

    [[nodiscard]] VkBuffer cameraUboBuffer() const { return cameraUboBuffer_; }

    void resetDescriptorSetLayout(const VkDescriptorSetLayout cameraUboLayout)
    {
      if (cameraUboLayout_) {
        vkDestroyDescriptorSetLayout(device_, cameraUboLayout_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("MhZAjM8Ol8", "Camera UBO: vkDestroyDescriptorSetLayout(), H={}", static_cast<void *>(cameraUboLayout_));
        }
      }
      cameraUboLayout_ = cameraUboLayout;
    }

    [[nodiscard]] VkDescriptorSetLayout cameraUboLayout() const { return cameraUboLayout_; }

    void resetCameraUboMemory(const VkDeviceMemory cameraUboMemory)
    {
      if (cameraUboMemory_) {
        resetCameraUboMemoryRef(nullptr);
        vkFreeMemory(device_, cameraUboMemory_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("sZDmhVO33h", "Camera UBO: vkFreeMemory(), H={}", static_cast<void *>(cameraUboMemory_));
        }
      }
      cameraUboMemory_ = cameraUboMemory;
    }

    [[nodiscard]] VkDeviceMemory cameraUboMemory() const { return cameraUboMemory_; }

    void resetCameraUboMemoryRef(void *cameraUboMemoryRef)
    {
      if (cameraUboMemoryRef_) {
        vkUnmapMemory(device_, cameraUboMemory_);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("obN79x4SoH",
                                    "Camera UBO: vkUnmapMemory(), H={}, ref={}",
                                    static_cast<void *>(cameraUboMemory_),
                                    cameraUboMemoryRef_);
        }
      }
      cameraUboMemoryRef_ = cameraUboMemoryRef;
    }

    [[nodiscard]] void *cameraUboMemoryRef() const { return cameraUboMemoryRef_; }
  };

} // namespace vul::pipeline
