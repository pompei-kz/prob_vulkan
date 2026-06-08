//
// Created by pompei on 2026-06-08.
//

#pragma once
#include "app/Settings.h"
#include "di/Getter.h"

namespace app {

  class AppInitializer
  /// Bean application app::Settings vul::Print
  {
    getter::Getter<app::Settings> &setting_;

  public:
    explicit AppInitializer(getter::Getter<Settings> &setting);
  };
} // namespace app
