//
// Created by pompei on 2026-06-10.
//

#pragma once

namespace cmd::model {

  /**
   * Clipping planes of the camera view frustum.
   */
  struct Planes
  {

    /**
     * Distance from the camera to the near-clipping plane.
     *
     * Geometry closer than this distance will not be rendered.
     */
    float near;

    /**
     * Distance from the camera to the far clipping plane.
     *
     * Geometry farther than this distance will not be rendered.
     */
    float far;
  };

} // namespace cmd::model
