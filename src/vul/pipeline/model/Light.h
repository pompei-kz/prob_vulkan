//
// Created by pompei on 2026-06-12.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS

#include "LightType.h"

#include <glm/vec3.hpp>

namespace vul::pipeline::model {

  struct alignas(16) Light
  {
    // Смещение 0  | Размер 16 байт (Выравнивание 16)
    glm::vec3 color;

    // Смещение 12 | Размер 4 байта (Выравнивание 16)
    float force;

    // Смещение 16 | Размер 12 байт (Выравнивание 16)
    alignas(16) glm::vec3 direction;

    // Смещение 28 | Размер 4 байта. Идеально ложится в «пустой хвост» после direction!
    LightType type;

    // Смещение 32 | Размер 12 байт (Выравнивание 16)
    alignas(16) glm::vec3 position;

    // Смещение 44 | Размер 4 байта. Идеально ложится в «пустой хвост» после position!
    float radius;
  };

  static_assert(sizeof(Light) == 48, "Размер Light должен быть ровно 48 байт");
  static_assert(offsetof(Light, color) == 0, "Смещение color должно быть 0");
  static_assert(offsetof(Light, force) == 12, "Смещение force должно быть 12");
  static_assert(offsetof(Light, direction) == 16, "Смещение direction должно быть 16");
  static_assert(offsetof(Light, type) == 28, "Смещение type должно быть 28");
  static_assert(offsetof(Light, position) == 32, "Смещение position должно быть 32");
  static_assert(offsetof(Light, radius) == 44, "Смещение radius должно быть 44");

} // namespace vul::pipeline::model
