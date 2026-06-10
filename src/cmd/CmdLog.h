//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Cmd.h"

#include <functional>
#include <string>

namespace cmd {

  struct CmdLog : Cmd
  {
    std::string_view             placeId;
    std::function<std::string()> messageFunc;

    CmdLog() {}

    CmdLog(const std::string_view placeId, const std::function<std::string()> &messageFunc)
        : placeId(placeId)
        , messageFunc(messageFunc)
    {}
  };

} // namespace cmd
