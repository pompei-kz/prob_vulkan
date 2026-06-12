//
// Created by pompei on 2026-06-12.
//

#pragma once
#include "Light.h"

#include <cstdint>

namespace vul::pipeline::model {

  struct alignas(16) LightsUbo
  {
    uint32_t count;
    uint32_t _pad[3];
    alignas(16) Light lights[10];
  };

  static_assert(sizeof(LightsUbo) == 496, "Размер LightsUbo должен быть ровно 496 байт (16 + 480)");
  static_assert(offsetof(LightsUbo, count) == 0, "Смещение count должно быть 0");
  static_assert(offsetof(LightsUbo, lights) == 16, "Смещение массива lights должно быть ровно 16");

} // namespace vul::pipeline::model
