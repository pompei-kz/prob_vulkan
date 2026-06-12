//
// Created by pompei on 2026-06-12.
//

#pragma once
#include <cstdint>

namespace vul::pipeline::model {

  enum class LightType : uint32_t {
    /**
     * Солнечный свет
     */
    SUN = 0,

    /**
     * Точечный свет
     */
    POINT = 1
  };

} // namespace vul::pipeline::model
