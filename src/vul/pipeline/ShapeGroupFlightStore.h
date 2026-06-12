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

    VkDescriptorSet descriptorSet_ = VK_NULL_HANDLE;

  public:
    explicit ShapeGroupFlightStore(const VkDevice device)
        : device_(device)
    {}

    ~ShapeGroupFlightStore() { resetDescriptorPool(VK_NULL_HANDLE); }

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

    void populate(const VkPhysicalDevice physicalDevice, const std::string &pipelineID, const VkDescriptorSetLayout descriptorSetLayout)
    {
      camera_ = std::make_unique<UboBufferStore>(device_, physicalDevice, "Camera");
      camera_->populateUboBufferStore(pipelineID, sizeof(model::CameraUbo));
      lights_ = std::make_unique<UboBufferStore>(device_, physicalDevice, "Lights");
      lights_->populateUboBufferStore(pipelineID, sizeof(model::LightsUbo));

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
                                    "Pipeline_ShapeGroup[{}]: vkCreateDescriptorPool() called OK, H={}",
                                    pipelineID,
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
                                    "Pipeline_ShapeGroup[{}]: vkAllocateDescriptorSets() called OK, H={}",
                                    pipelineID,
                                    static_cast<void *>(descriptorSet));
        }
      }
    }
  };

} // namespace vul::pipeline