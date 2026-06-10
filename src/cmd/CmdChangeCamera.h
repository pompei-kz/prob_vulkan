//
// Created by pompei on 2026-06-10.
//

#pragma once

#include "model/Planes.h"

#include <glm/vec3.hpp>
#include <string>

namespace cmd {

  /**
   * Command that updates camera parameters.
   *
   * Each camera property has a corresponding to apply flag.
   * A property is updated only if its apply flag is set to true.
   */
  struct CmdChangeCamera
  {

    /**
     * Camera identifier pointed to camera to change.
     *
     * If this ID does not point to a valid camera, the command will be ignored.
     *
     * If ID is empty, the command will be applied to the default camera.
     */
    std::string cameraId;

    /**
     * Apply camera position.
     */
    bool positionApply = false;

    /**
     * New camera position in world space.
     */
    glm::vec3 position{0.0F, 0.0F, 1.0F};

    /**
     * Apply camera forward direction.
     */
    bool forwardApply = false;

    /**
     * Forward direction of the camera.
     * This vector defines the direction the camera is looking toward.
     */
    glm::vec3 forward{0.0F, 0.0F, -1.0F};

    /**
     * Apply camera up direction.
     */
    bool upApply = false;

    /**
     * Up direction of the camera.
     *
     * This vector does not have to be normalized or perpendicular to
     * `forward`. The system will recalculate it so that the resulting
     * up vector is normalized and perpendicular to `forward`.
     *
     * The original `up`, `forward`, and recalculated `up` vectors must lie
     * on the same plane.
     */
    glm::vec3 up{0.0F, 1.0F, 0.0F};

    /**
     * Apply near and far clipping plane parameters.
     */
    bool planesApply = false;

    /**
     * Near and far clipping planes of the camera frustum.
     * Only geometry located between these planes is rendered.
     */
    model::Planes planes;

    /**
     * Apply field of view.
     */
    bool fovDegreesApply = false;

    /**
     * Vertical field of view in degrees.
     */
    float fovDegrees = 45.0F;
  };
} // namespace cmd
