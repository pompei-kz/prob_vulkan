#pragma once
#include "app/Settings.h"
#include "di/Getter.h"
#include "vul/Print.h"

namespace app {

  class AppInitializer
  /// BEAN appInitializer app::Settings vul::Print
  {
    di::Getter<Settings> &setting_;
    di::Getter<vul::Print> &print_;

  public:
    explicit AppInitializer(di::Getter<Settings> &setting, di::Getter<vul::Print> &print);

    void init();
  };
} // namespace app
