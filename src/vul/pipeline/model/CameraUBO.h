//
// Created by pompei on 2026-06-11.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace vul::pipeline::model {

  struct CameraUBO
  {
    glm::mat4 view;
    glm::mat4 projection;

    alignas(16) glm::vec4 lightDirectionAndForce;
  };

} // namespace vul::pipeline::model
