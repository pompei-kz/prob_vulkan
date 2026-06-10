//
// Created by pompei on 2026-06-10.
//

#include "Pipeline_ShapesGroup_Worker.h"

#include "Pipeline_ShapeGroup.h"

namespace vul::pipeline {

  void Pipeline_ShapesGroup_Worker::execute(const cmd::CmdSetPipeline_ShapeGroup *cmd)
  {
    auto pipeline = std::unique_ptr<vul::pipeline::Pipeline_ShapeGroup>();
    populatePipeline(pipeline, cmd);
    handleStore_->pipelines()->put(cmd->id, std::move(pipeline));
  }

  void Pipeline_ShapesGroup_Worker::populatePipeline(const std::unique_ptr<vul::pipeline::Pipeline_ShapeGroup> &pipeline, const cmd::CmdSetPipeline_ShapeGroup *cmd)
  {
    handleStore_->swapChain();
  }
} // namespace vul::pipeline