//
// Created by pompei on 2026-06-10.
//

#include "Pipeline_ShapesGroup_Worker.h"
#include "Pipeline_ShapeGroup.h"
#include "model/CameraUBO.h"
#include "util/util.h"

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
      util::Log::get()->verbose("PNQoYMSkew", "Start create Pipeline_ShapeGroup, id={}", cmd->id);
    }

    createVkCommandPool(pipeline, cmd);

    createVkCommandBuffers(pipeline, cmd);

    createCameraUboBuffer(pipeline, cmd);

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

    pipeline->resetVkCommandPool(commandPool);

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("t1RBFCyIOQ", "Pipeline_ShapeGroup[{}]: vkCreateCommandPool({})", cmd->id, static_cast<void *>(commandPool));
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
      for (auto commandBuffer : commandBuffers) {
        if (str.length() > 0) str += ", ";
        str += std::format("{}", static_cast<void *>(commandBuffer));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("5u5xWeq8l6", "Pipeline_ShapeGroup[{}]: vkAllocateCommandBuffers({})", cmd->id, str);
      }
    }

    pipeline->resetVkCommandBuffers(commandBuffers);
  }

  void Pipeline_ShapesGroup_Worker::createCameraUboBuffer(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                          const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    const VkDevice device = topStore_->device()->handle();

    VkBufferCreateInfo ci{};
    ci.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ci.size        = sizeof(model::CameraUBO);
    ci.usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;

    if (const VkResult result = vkCreateBuffer(device, &ci, nullptr, &buffer); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("OFq7HkWThf :: failed to create camera uniform buffer: VkResult = ") + util::VkResult_to_str(result));
    }

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("Li3pcANb5m", "Camera UBO: vkCreateBuffer[{}] : {}", cmd->id, static_cast<void *>(buffer));
    }

    pipeline->resetCameraUboBuffer(buffer);
  }

} // namespace vul::pipeline