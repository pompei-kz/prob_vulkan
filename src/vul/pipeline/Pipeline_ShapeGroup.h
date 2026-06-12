//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline.h"
#include "ShapeGroupFlightStore.h"
#include "util/Log.h"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace vul::pipeline {

  class Pipeline_ShapeGroup : public Pipeline
  {
  public:
    static constexpr int FRAMES_IN_FLIGHT = 2;

  private:
    VkCommandPool                commandPool_ = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers_;

    VkDescriptorSetLayout descriptorSetLayout_ = VK_NULL_HANDLE;

  public:
    std::vector<std::unique_ptr<ShapeGroupFlightStore>> flights;

    explicit Pipeline_ShapeGroup(const VkDevice vkDevice)
        : Pipeline(vkDevice)
    {}

    ~Pipeline_ShapeGroup() override { resetCommandPool(VK_NULL_HANDLE); }



    void resetCommandPool(const VkCommandPool commandPool)
    {
      if (commandPool_) {
        resetVkCommandBuffers(std::vector<VkCommandBuffer>());
        resetDescriptorSetLayout(VK_NULL_HANDLE);

        vkDestroyCommandPool(device_, commandPool_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("rFW7XX7zgk", "vkDestroyCommandPool(), H={}", static_cast<void *>(commandPool_));
        }
      }

      commandPool_ = commandPool;
    }

    [[nodiscard]] VkCommandPool vkCommandPool() const { return commandPool_; }

    void resetVkCommandBuffers(const std::vector<VkCommandBuffer> &commandBuffers)
    {
      if (commandBuffers_.size() > 0) {
        vkFreeCommandBuffers(device_, commandPool_, commandBuffers_.size(), commandBuffers_.data());

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

    void resetDescriptorSetLayout(const VkDescriptorSetLayout descriptorSetLayout)
    {
      if (descriptorSetLayout_) {
        vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("MhZAjM8Ol8", "vkDestroyDescriptorSetLayout(), H={}", static_cast<void *>(descriptorSetLayout_));
        }
      }
      descriptorSetLayout_ = descriptorSetLayout;
    }

    [[nodiscard]] VkDescriptorSetLayout descriptorSetLayout() const { return descriptorSetLayout_; }
  };

} // namespace vul::pipeline
