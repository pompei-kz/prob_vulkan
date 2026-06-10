//
// Created by pompei on 2026-06-10.
//

#include "ExecuteCmd.h"

#include "CmdLog.h"
#include "CmdPipeline.h"
#include "CmdSequence.h"
#include "util/Log.h"
#include "util/util.h"
#include "vul/pipeline/Pipeline_ShapeGroup.h"

namespace cmd {
  // ReSharper disable once CppPassValueParameterByConstReference
  void ExecuteCmd::execute(const CmdPtr cmdPtr)
  {
    try {
      execute_Cmd(cmdPtr);
    } catch (const std::exception &e) {
      util::Log::get()->error("6HPuJMEFei", "Cmd exception: {}", e.what());
    } catch (...) {
      util::Log::get()->error("PD9Q2EnyAz", "Cmd unknown exception");
    }
  }

  // ReSharper disable once CppPassValueParameterByConstReference
  void ExecuteCmd::execute_Cmd(const CmdPtr cmdPtr)
  {
    if (!cmdPtr) return;

    if (const CmdNop *_ = dynamic_cast<CmdNop *>(cmdPtr.get())) {
      // Nothing to do: cmd::CmdNop is no operation
      return;
    }

    if (const CmdSequence *cmd = dynamic_cast<CmdSequence *>(cmdPtr.get())) {
      for (const CmdPtr subCmdPtr : cmd->sequence) {
        execute_Cmd(subCmdPtr);
      }
      return;
    }

    if (const CmdLog *cmd = dynamic_cast<CmdLog *>(cmdPtr.get())) {
      execute_CmdLog(cmd);
      return;
    }

    if (const CmdSetPipeline_ShapeGroup *cmd = dynamic_cast<CmdSetPipeline_ShapeGroup *>(cmdPtr.get())) {
      execute_CmdSetPipeline_ShapeGroup(cmd);
      return;
    }

    util::Log::get()->error("G6MW6d6ZGK", "Unknown command {}", (typeid(*cmdPtr)).name());
  }

  void ExecuteCmd::execute_CmdLog(const CmdLog *cmd) const
  {
    const std::string_view placeId = cmd->placeId;
    const std::string      msg     = cmd->message;

    switch (cmd->severity) {
      case Severity::ERROR: log_->error(placeId, "CmdLog {}", msg); break;
      case Severity::WARNING: log_->warn(placeId, "CmdLog {}", msg); break;
      case Severity::INFO: log_->info(placeId, "CmdLog {}", msg); break;
      default: log_->verbose(placeId, "CmdLog {}", msg); break;
    }
  }

  void ExecuteCmd::execute_CmdSetPipeline_ShapeGroup(const CmdSetPipeline_ShapeGroup *cmd)
  {

    pipeline_shapesGroup_worker_->execute(cmd);

    auto pipeline = std::unique_ptr<vul::pipeline::Pipeline_ShapeGroup>();
    populatePipeline(pipeline, cmd);
    handleStore_->pipelines()->put(cmd->id, std::move(pipeline));
  }

  void ExecuteCmd::populatePipeline(const std::unique_ptr<vul::pipeline::Pipeline_ShapeGroup> &pipeline, const CmdSetPipeline_ShapeGroup *cmd) const
  {
    VkCommandPoolCreateInfo ci{};
    ci.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.queueFamilyIndex = handleStore_->device()->graphicsFamilyIndex();
    ci.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkCommandPool vkCommandPool;

    if (const VkResult result = vkCreateCommandPool(handleStore_->device()->handle(), &ci, nullptr, &vkCommandPool); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("GO6RRSCwQJ :: failed to create logical device: VkResult = ") + util::VkResult_to_str(result));
    }
  }
} // namespace cmd
