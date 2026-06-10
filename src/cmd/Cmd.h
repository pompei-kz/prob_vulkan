//
// Created by pompei on 2026-06-10.
//

#pragma once
#include <functional>
#include <memory>

namespace cmd {

  struct Cmd
  {
    virtual ~Cmd();
  };

  using CmdPtr     = std::shared_ptr<Cmd>;
  using CmdFactory = std::function<CmdPtr()>;

  struct CmdNop : Cmd
  {
  };

} // namespace cmd
