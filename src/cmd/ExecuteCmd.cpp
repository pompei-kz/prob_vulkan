//
// Created by pompei on 2026-06-10.
//

#include "ExecuteCmd.h"

#include "CmdSequence.h"
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

    util::Log::get()->error("G6MW6d6ZGK", "Unknown command {}", (typeid(*cmdPtr)).name());
  }
} // namespace cmd
