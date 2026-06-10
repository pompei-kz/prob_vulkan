//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Cmd.h"

namespace cmd {

  struct CmdSequence : Cmd
  {
    std::vector<CmdPtr> sequence;
  };

} // namespace cmd
