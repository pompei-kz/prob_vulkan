//
// Created by pompei on 2026-06-12.
//

#pragma once
#include "UboBufferStore.h"
#include "model/CameraUBO.h"
#include "model/LightsUbo.h"

#include <string>
#include <vulkan/vulkan_core.h>

namespace vul::pipeline {

  class ShapeGroupFlightStore
  {
    const VkDevice device_;

    std::unique_ptr<UboBufferStore> camera_{};
    std::unique_ptr<UboBufferStore> lights_{};

    VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;

    VkDescriptorSet descriptorSet_           = VK_NULL_HANDLE;
    VkSemaphore     imageAvailableSemaphore_ = VK_NULL_HANDLE;
    VkSemaphore     renderFinishedSemaphore_ = VK_NULL_HANDLE;
    VkFence         inFlightFence_           = VK_NULL_HANDLE;

  public:
    explicit ShapeGroupFlightStore(const VkDevice device)
        : device_(device)
    {}

    ~ShapeGroupFlightStore()
    {
      resetInFlightFence(VK_NULL_HANDLE);
      resetImageAvailableSemaphore(VK_NULL_HANDLE);
      resetRenderFinishedSemaphore(VK_NULL_HANDLE);
      resetDescriptorPool(VK_NULL_HANDLE);
    }

    void resetImageAvailableSemaphore(const VkSemaphore imageAvailableSemaphore)
    {
      if (imageAvailableSemaphore_) {
        vkDestroySemaphore(device_, imageAvailableSemaphore_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("pVqz3S9dW2M", "vkDestroySemaphore(imageAvailableSemaphore), H={}", static_cast<void *>(imageAvailableSemaphore_));
        }
      }
      imageAvailableSemaphore_ = imageAvailableSemaphore;
    }

    void resetRenderFinishedSemaphore(const VkSemaphore renderFinishedSemaphore)
    {
      if (renderFinishedSemaphore_) {
        vkDestroySemaphore(device_, renderFinishedSemaphore_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("TL0ybX9We4", "vkDestroySemaphore(renderFinishedSemaphore), H={}", static_cast<void *>(renderFinishedSemaphore_));
        }
      }
      renderFinishedSemaphore_ = renderFinishedSemaphore;
    }

    void resetInFlightFence(const VkFence inFlightFence)
    {
      if (inFlightFence_) {
        vkDestroyFence(device_, inFlightFence_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("p7x3AZ8GdN", "vkDestroyFence(inFlightFence), H={}", static_cast<void *>(inFlightFence_));
        }
      }
      inFlightFence_ = inFlightFence;
    }

    void resetDescriptorPool(const VkDescriptorPool descriptorPool)
    {
      if (descriptorPool_) {
        setDescriptorSet(VK_NULL_HANDLE);

        vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("iah66xp7a8", "vkDestroyDescriptorPool(), H={}", static_cast<void *>(descriptorPool_));
        }
      }
      descriptorPool_ = descriptorPool;
    }

    [[nodiscard]] VkDescriptorPool descriptorPool() const { return descriptorPool_; }

    void setDescriptorSet(const VkDescriptorSet descriptorSet) { descriptorSet_ = descriptorSet; }

    [[nodiscard]] VkDescriptorSet descriptorSet() const { return descriptorSet_; }

    void populate(const VkPhysicalDevice      physicalDevice,
                  const std::string          &pipelineID,
                  const VkDescriptorSetLayout descriptorSetLayout,
                  int                         flightIndex)
    {
      camera_ = std::make_unique<UboBufferStore>(device_, physicalDevice, "Camera");
      camera_->populateUboBufferStore(pipelineID, flightIndex, sizeof(model::CameraUbo));
      lights_ = std::make_unique<UboBufferStore>(device_, physicalDevice, "Lights");
      lights_->populateUboBufferStore(pipelineID, flightIndex, sizeof(model::LightsUbo));

      {
        VkDescriptorPoolSize cameraUboPoolSize{};
        cameraUboPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        cameraUboPoolSize.descriptorCount = 1;

        VkDescriptorPoolSize lightsUboPoolSize{};
        lightsUboPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        lightsUboPoolSize.descriptorCount = 1;

        const std::array<VkDescriptorPoolSize, 2> descriptorPoolSizes = {cameraUboPoolSize, lightsUboPoolSize};

        VkDescriptorPoolCreateInfo ci{};
        ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        ci.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
        ci.maxSets       = 1;
        ci.pPoolSizes    = descriptorPoolSizes.data();

        VkDescriptorPool descriptorPool;

        if (const VkResult result = vkCreateDescriptorPool(device_, &ci, nullptr, &descriptorPool); result != VK_SUCCESS) {
          throw std::runtime_error(std::string("RMxq0rPr36 :: ERROR calling vkCreateDescriptorPool(): VkResult = ") + util::VkResult_to_str(result));
        }

        resetDescriptorPool(descriptorPool);

        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("e7Ep1Js9u6",
                                    "Pipeline_ShapeGroup[{}: {}]: vkCreateDescriptorPool() called OK, H={}",
                                    pipelineID,
                                    flightIndex,
                                    static_cast<void *>(descriptorPool));
        }
      }

      {
        VkDescriptorSetAllocateInfo dsa{};
        dsa.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        // ReSharper disable once CppDFANotInitializedField
        dsa.descriptorPool     = descriptorPool_;
        dsa.descriptorSetCount = 1;
        dsa.pSetLayouts        = &descriptorSetLayout;

        VkDescriptorSet descriptorSet;

        if (const VkResult result = vkAllocateDescriptorSets(device_, &dsa, &descriptorSet); result != VK_SUCCESS) {
          throw std::runtime_error(std::string("MLuC1o1UXJ :: ERROR calling vkAllocateDescriptorSets(): VkResult = ") +
                                   util::VkResult_to_str(result));
        }

        setDescriptorSet(descriptorSet);

        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("VTtxMhPDOf",
                                    "Pipeline_ShapeGroup[{}: {}]: vkAllocateDescriptorSets() called OK, H={}",
                                    pipelineID,
                                    flightIndex,
                                    static_cast<void *>(descriptorSet));
        }
      }

      {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore sema;

        if (const VkResult result = vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &sema); result != VK_SUCCESS) {
          throw std::runtime_error(std::format("XL6A6mwMSe :: ERROR calling vkCreateSemaphore(ImageAvailableSemaphore): VkResult = {}", //
                                               util::VkResult_to_str(result)));
        }

        resetImageAvailableSemaphore(sema);

        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("AY45gyXpeS", //
                                    "Pipeline_ShapeGroup[{}: {}]: vkCreateSemaphore(ImageAvailableSemaphore) called OK, H={}",
                                    pipelineID,
                                    flightIndex,
                                    static_cast<void *>(sema));
        }
      }
      {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore sema;

        if (const VkResult result = vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &sema); result != VK_SUCCESS) {
          throw std::runtime_error(std::format("inc1fzUDek :: ERROR calling vkCreateSemaphore(RenderFinishedSemaphore): VkResult = {}", //
                                               util::VkResult_to_str(result)));
        }

        resetRenderFinishedSemaphore(sema);

        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("ycfOJX46hM", //
                                    "Pipeline_ShapeGroup[{}: {}]: vkCreateSemaphore(RenderFinishedSemaphore) called OK, H={}",
                                    pipelineID,
                                    flightIndex,
                                    static_cast<void *>(sema));
        }
      }
      {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;

        if (const VkResult result = vkCreateFence(device_, &fenceInfo, nullptr, &fence); result != VK_SUCCESS) {
          throw std::runtime_error(std::format("inc1fzUDek :: ERROR calling vkCreateFence(InFlightFence): VkResult = {}", //
                                               util::VkResult_to_str(result)));
        }

        resetInFlightFence(fence);

        if (util::Log::get()->hasVerbose()) {
          util::Log::get()->verbose("E7218HeKPv", //
                                    "Pipeline_ShapeGroup[{}: {}]: vkCreateFence(InFlightFence) called OK, H={}",
                                    pipelineID,
                                    flightIndex,
                                    static_cast<void *>(fence));
        }
      }
    }
  };

} // namespace vul::pipeline