//
// Created by pompei on 2026-06-10.
//

#include "ExecuteCmd.h"

#include "util/Log.h"

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
    Cmd *cmd = cmdPtr.get();

    if (!cmd) return;

    if (typeid(*cmd) == typeid(Cmd)) {
      // Nothing to do: cmd::Cmd is no operation command
      return;
    }

    util::Log::get()->error("G6MW6d6ZGK", "Unknown command {}", typeid(cmd).name());
  }
} // namespace cmd
