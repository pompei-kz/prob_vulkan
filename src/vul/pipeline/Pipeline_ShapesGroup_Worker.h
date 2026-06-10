//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline_ShapeGroup.h"
#include "cmd/CmdPipeline.h"
#include "vul/HandleStore.h"

#include <memory>

namespace vul::pipeline {

  class Pipeline_ShapesGroup_Worker
  /// BEAN pipeline_ShapesGroup_Worker vul::HandleStore
  {

    di::Getter<vul::HandleStore> *handleStore_;

  public:
    Pipeline_ShapesGroup_Worker(di::Getter<HandleStore> *handleStore)
        : handleStore_(handleStore)
    {}

    void execute(const cmd::CmdSetPipeline_ShapeGroup *cmd);

  private:
    void populatePipeline(const std::unique_ptr<Pipeline_ShapeGroup> &pipeline, const cmd::CmdSetPipeline_ShapeGroup *cmd);
  };

} // namespace vul::pipeline