//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Cmd.h"

#include <functional>
#include <string>

namespace cmd {

  enum class Severity {
    ERROR,
    WARNING,
    INFO,
    VERBOSE,
  };

  struct CmdLog : Cmd
  {
    std::string_view placeId;
    Severity         severity = Severity::VERBOSE;
    std::string      message;

    explicit CmdLog()
        : placeId("")
        , message("")
    {}

    explicit CmdLog(const std::string_view placeId, const Severity severity, const std::string &message)
        : placeId(placeId)
        , severity(severity)
        , message(message)
    {}

    explicit CmdLog(const std::string_view placeId, const std::string &message)
        : placeId(placeId)
        , message(message)
    {}

    static CmdLog error(const std::string_view placeId, const std::string &message)
    {
      return CmdLog(placeId, Severity::ERROR, message);
    }

    static CmdLog warn(const std::string_view placeId, const std::string &message)
    {
      return CmdLog(placeId, Severity::WARNING, message);
    }

    static CmdLog info(const std::string_view placeId, const std::string &message)
    {
      return CmdLog(placeId, Severity::INFO, message);
    }

    static CmdLog verbose(const std::string_view placeId, const std::string &message)
    {
      return CmdLog(placeId, Severity::VERBOSE, message);
    }
  };

} // namespace cmd
