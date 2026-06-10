//
// Created by pompei on 2026-06-10.
//

#pragma once
#include <functional>
#include <string>

namespace cmd {

  class CmdLog
  {
    std::string_view placeId_;
    std::function<std::string()> func_;

  public:
    explicit CmdLog(std::string_view placeId, const std::function<std::string()> &func)
        : func_(func)
    {}
  };

} // namespace cmd
