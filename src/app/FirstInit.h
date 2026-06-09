//
// Created by pompei on 2026-06-09.
//

#pragma once

namespace app {
  class FirstInit
  /// BEAN firstInit
  {
    bool initialized_ = false;
    bool destroyed_ = false;
  public:

    void init();

    void  destroy();
  };
}
