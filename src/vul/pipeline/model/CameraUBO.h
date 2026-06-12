//
// Created by pompei on 2026-06-11.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace vul::pipeline::model {

  struct alignas(16) CameraUbo
  {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    float     _pad[1];
  };

  static_assert(sizeof(CameraUbo) == 144, "Размер CameraUbo должен быть ровно 144 байта (3 * 4 * 16 байт)");
  static_assert(offsetof(CameraUbo, view) == 0, "Смещение view должно быть 0");
  static_assert(offsetof(CameraUbo, projection) == 64, "Смещение projection должно быть 64");
  static_assert(offsetof(CameraUbo, position) == 128, "Смещение position должно быть 128");

} // namespace vul::pipeline::model
