//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Cmd.h"
#include "CmdLog.h"
#include "CmdPipeline.h"
#include "util/Log.h"
#include "vul/TopStore.h"
#include "vul/pipeline/Pipeline_ShapeGroup.h"
#include "vul/pipeline/Pipeline_ShapesGroup_Worker.h"

#include <memory>

namespace cmd {

  class ExecuteCmd
  /// BEAN executeCmd
  /// DEPS vul::TopStore vul::pipeline::Pipeline_ShapesGroup_Worker
  {
    util::Log *log_ = util::Log::get();

    di::Getter<vul::TopStore>                              &topStore_;
    di::Getter<vul::pipeline::Pipeline_ShapesGroup_Worker> &pipeline_shapesGroup_worker_;

  public:
    ExecuteCmd(di::Getter<vul::TopStore> &topStore, di::Getter<vul::pipeline::Pipeline_ShapesGroup_Worker> &pipeline_ShapesGroup_Worker)
        : topStore_(topStore)
        , pipeline_shapesGroup_worker_(pipeline_ShapesGroup_Worker)
    {}

    void execute(const CmdPtr cmdPtr);

  private:
    void execute_Cmd(const CmdPtr cmdPtr);
    void execute_CmdLog(const CmdLog *cmd) const;
  };

} // namespace cmd
