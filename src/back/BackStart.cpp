//
// Created by pompei on 2026-06-10.
//

#include "BackStart.h"

#include "cmd/CmdLog.h"
#include "cmd/CmdPipeline.h"

namespace back {
  // ReSharper disable once CppMemberFunctionMayBeStatic
  cmd::CmdPtr BackStart::start()
  {
    auto cmd = std::make_shared<cmd::CmdSetPipeline_ShapeGroup>();
    cmd->id = "BackStart";



    return cmd;
  }
} // namespace back
