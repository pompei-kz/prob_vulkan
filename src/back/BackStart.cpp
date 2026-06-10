//
// Created by pompei on 2026-06-10.
//

#include "BackStart.h"

#include "cmd/CmdLog.h"

namespace back {
  // ReSharper disable once CppMemberFunctionMayBeStatic
  cmd::CmdPtr BackStart::start()
  {
    return std::make_unique<cmd::CmdLog>("GuNxKIz1xC", [] { return "Hello World"; });
  }
} // namespace back