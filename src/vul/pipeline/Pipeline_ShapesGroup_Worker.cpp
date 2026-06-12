//
// Created by pompei on 2026-06-10.
//

#include "Pipeline_ShapesGroup_Worker.h"
#include "Pipeline_ShapeGroup.h"
#include "model/CameraUBO.h"
#include "util/util.h"
#include "vul/ImageStore_Worker.h"

namespace vul::pipeline {

  void Pipeline_ShapesGroup_Worker::execute(const cmd::CmdSetPipeline_ShapeGroup *cmd) const
  {
    if (cmd->id.empty()) {
      throw std::runtime_error("9AmDQ9jd4n :: Pipeline_ShapesGroup_Worker::execute: cmd->id is empty");
    }

    auto pipeline = std::make_unique<Pipeline_ShapeGroup>(topStore_->device()->handle());
    populatePipeline(pipeline, cmd);
    topStore_->device()->pipelines()->put(cmd->id, std::move(pipeline));
  }

  void Pipeline_ShapesGroup_Worker::populatePipeline(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                     const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("PNQoYMSkew", "Pipeline_ShapeGroup[{}]: Start create Pipeline_ShapeGroup", cmd->id);
    }

    createVkCommandPool(pipeline, cmd);
    createVkCommandBuffers(pipeline, cmd);
    createDescriptorSetLayout(pipeline, cmd);
    createDescriptorSetLayout(pipeline, cmd);
    createFrameFlights(pipeline, cmd);

    // TODO Продолжить здесь
  }

  void Pipeline_ShapesGroup_Worker::createVkCommandPool(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                        const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = topStore_->device()->queueFamilyIndices().indexGraphics.value();
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool commandPool;

    if (const VkResult result = vkCreateCommandPool(topStore_->device()->handle(), &ci, nullptr, &commandPool); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("GO6RRSCwQJ :: failed to create command pool: VkResult = ") + util::VkResult_to_str(result));
    }

    pipeline->resetCommandPool(commandPool);

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("t1RBFCyIOQ",
                                "Pipeline_ShapeGroup[{}]: vkCreateCommandPool() called OK, H={}",
                                cmd->id,
                                static_cast<void *>(commandPool));
    }
  }

  void Pipeline_ShapesGroup_Worker::createVkCommandBuffers(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                           const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    std::vector<VkCommandBuffer> commandBuffers;

    commandBuffers.resize(topStore_->swapChain()->swapChainImages().size());

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("0Ne2n3xjPn", "Pipeline_ShapeGroup[{}]: commandBuffers.size = {}", cmd->id, commandBuffers.size());
    }

    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = pipeline->vkCommandPool();
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = commandBuffers.size();

    if (const VkResult result = vkAllocateCommandBuffers(topStore_->device()->handle(), &ai, commandBuffers.data()); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("lwVho03Xia :: failed to allocate command buffers: VkResult = ") + util::VkResult_to_str(result));
    }

    if (util::Log::get()->hasInfo()) {
      std::string str;
      for (const auto commandBuffer : commandBuffers) {
        if (str.length() > 0) str += ", ";
        str += std::format("{}", static_cast<void *>(commandBuffer));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("5u5xWeq8l6", "Pipeline_ShapeGroup[{}]: vkAllocateCommandBuffers() called OK, H={}", cmd->id, str);
      }
    }

    pipeline->resetVkCommandBuffers(commandBuffers);
  }

  void Pipeline_ShapesGroup_Worker::createDescriptorSetLayout(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                              const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    const VkDevice device = topStore_->device()->handle();

    std::vector<VkDescriptorSetLayoutBinding> bindings{};
    bindings.resize(2);

    {
      bindings[0].binding            = 0;
      bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      bindings[0].descriptorCount    = 1;
      bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      bindings[0].pImmutableSamplers = nullptr;

      bindings[1].binding            = 1;
      bindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      bindings[1].descriptorCount    = 1;
      bindings[1].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      bindings[1].pImmutableSamplers = nullptr;
    }

    VkDescriptorSetLayoutCreateInfo ci{};
    ci.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.bindingCount = bindings.size();
    ci.pBindings    = bindings.data();

    VkDescriptorSetLayout layout{};

    if (const VkResult result = vkCreateDescriptorSetLayout(device, &ci, nullptr, &layout); result != VK_SUCCESS) {
      throw std::runtime_error(std::format("lwVho03Xia :: ERROR calling vkCreateDescriptorSetLayout(): VkResult = {}", //
                                           util::VkResult_to_str(result)));
    }

    pipeline->resetDescriptorSetLayout(layout);

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("VphJq8B3uw",
                                "Pipeline_ShapeGroup[{}]: vkCreateDescriptorSetLayout() called OK, H={}",
                                cmd->id,
                                static_cast<void *>(layout));
    }
  }
  void Pipeline_ShapesGroup_Worker::createFrameFlights(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                       const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    const VkDevice device = topStore_->device()->handle();

    for (int i = 0; i < Pipeline_ShapeGroup::FRAMES_IN_FLIGHT; ++i) {
      auto flight = std::make_unique<ShapeGroupFlightStore>(device);
      flight->populate(topStore_->selectedVkPhysicalDevice(), cmd->id, pipeline->descriptorSetLayout(), i);
      pipeline->flights.push_back(std::move(flight));
    }
  }

} // namespace vul::pipeline