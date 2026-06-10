//
// Created by pompei on 2026-06-10.
//

#include "ExecuteCmd.h"

#include "util/Log.h"

namespace cmd {
  void ExecuteCmd::execute_Cmd(const CmdPtr cmdPtr)
  {
    Cmd *cmd = cmdPtr.get();

    if (!cmd) return;

    if (typeid(*cmd) == typeid(Cmd)) {
      // Nothing to do: cmd::Cmd is no operation command
      return;
    }

    util::Log::get()->error("G6MW6d6ZGK", "Unknown command {}", typeid(cmd).name());
  }
} // namespace cmd
