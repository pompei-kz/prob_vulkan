//
// Created by pompei on 2026-06-10.
//

#include "ExecuteCmd.h"

#include "CmdLog.h"
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

    if (const CmdLog *cmd = dynamic_cast<CmdLog *>(cmdPtr.get())) {

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
} // namespace cmd
