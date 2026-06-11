//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline_ShapeGroup.h"
#include "cmd/CmdPipeline.h"
#include "di/Getter.h"
#include "vul/HandleStore.h"

#include <memory>

namespace vul::pipeline {

  class Pipeline_ShapesGroup_Worker
  /// BEAN pipeline_ShapesGroup_Worker vul::HandleStore
  {
    di::Getter<HandleStore> &handleStore_;

  public:
    explicit Pipeline_ShapesGroup_Worker(di::Getter<HandleStore> &handleStore)
        : handleStore_(handleStore)
    {}

    void execute(const cmd::CmdSetPipeline_ShapeGroup *cmd) const;

  private:
    void populatePipeline(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline, const cmd::CmdSetPipeline_ShapeGroup *cmd) const;
    void createVkCommandPool(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline) const;
    void createVkCommandBuffers(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline) const;
  };

} // namespace vul::pipeline