//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Cmd.h"
#include "CmdLog.h"
#include "util/Log.h"

namespace cmd {

  class ExecuteCmd
  /// BEAN executeCmd
  {

    util::Log *log_ = util::Log::get();

  public:
    void execute(const CmdPtr cmdPtr);

  private:
    void execute_Cmd(const CmdPtr cmdPtr);
    void execute_CmdLog(const CmdLog *cmd) const;
  };

} // namespace cmd
