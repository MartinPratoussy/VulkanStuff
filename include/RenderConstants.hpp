/**
 * @file RenderConstants.hpp
 * @brief Named constants for rendering configuration
 * @details Centralizes magic numbers to make the codebase more maintainable
 */

#pragma once

#include <glm/glm.hpp>

/**
 * @namespace RenderConstants
 * @brief Configuration constants for the rendering pipeline
 */
namespace RenderConstants
{
    // === Camera Configuration ===

    /// Field of view angle in degrees for perspective projection
    constexpr float CAMERA_FOV_DEGREES = 45.0f;

    /// Camera position in world space
    constexpr glm::vec3 CAMERA_POSITION{2.0f, 2.0f, 2.0f};

    /// Point the camera is looking at
    constexpr glm::vec3 CAMERA_TARGET{0.0f, 0.0f, 0.0f};

    /// Camera up vector (Z-up coordinate system)
    constexpr glm::vec3 CAMERA_UP{0.0f, 0.0f, 1.0f};

    /// Near clipping plane distance
    constexpr float CAMERA_NEAR_PLANE = 0.1f;

    /// Far clipping plane distance
    constexpr float CAMERA_FAR_PLANE = 10.0f;

    // === Animation Configuration ===

    /// Rotation speed in degrees per second
    constexpr float ROTATION_SPEED_DEG_PER_SEC = 90.0f;

    /// Axis of rotation (Z-axis for spinning in XY plane)
    constexpr glm::vec3 ROTATION_AXIS{0.0f, 0.0f, 1.0f};

} // namespace RenderConstants
