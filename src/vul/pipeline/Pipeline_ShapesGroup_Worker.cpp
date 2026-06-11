//
// Created by pompei on 2026-06-10.
//

#include "Pipeline_ShapesGroup_Worker.h"
#include "Pipeline_ShapeGroup.h"
#include "util/util.h"

namespace vul::pipeline {

  void Pipeline_ShapesGroup_Worker::execute(const cmd::CmdSetPipeline_ShapeGroup *cmd) const
  {
    auto pipeline = std::make_unique<Pipeline_ShapeGroup>(topStore_->device()->handle());
    populatePipeline(pipeline, cmd);
    topStore_->pipelines()->put(cmd->id, std::move(pipeline));
  }

  void Pipeline_ShapesGroup_Worker::populatePipeline(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline,
                                                     const cmd::CmdSetPipeline_ShapeGroup       *cmd) const
  {
    createVkCommandPool(pipeline);

    createVkCommandBuffers(pipeline);

  }

  void Pipeline_ShapesGroup_Worker::createVkCommandPool(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline) const
  {
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = topStore_->device()->queueFamilyIndices().indexGraphics.value();
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool vkCommandPool;

    if (const VkResult result = vkCreateCommandPool(topStore_->device()->handle(), &ci, nullptr, &vkCommandPool); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("GO6RRSCwQJ :: failed to create command pool: VkResult = ") + util::VkResult_to_str(result));
    }

    pipeline->storeVkCommandPool(vkCommandPool);
  }

  void Pipeline_ShapesGroup_Worker::createVkCommandBuffers(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline) const
  {
    std::vector<VkCommandBuffer> commandBuffers;

    commandBuffers.resize(topStore_->swapChain()->swapChainImages().size());

    VkCommandBufferAllocateInfo ai{};
    ai.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool        = pipeline->vkCommandPool();
    ai.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = commandBuffers.size();

    if (const VkResult result = vkAllocateCommandBuffers(topStore_->device()->handle(), &ai, commandBuffers.data()); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("lwVho03Xia :: failed to allocate command buffers: VkResult = ") + util::VkResult_to_str(result));
    }
  }

} // namespace vul::pipeline