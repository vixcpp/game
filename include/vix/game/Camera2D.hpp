/**
 *
 *  @file Camera2D.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/game
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#ifndef VIX_GAME_CAMERA_2D_HPP
#define VIX_GAME_CAMERA_2D_HPP

#include <cstdint>

#include <vix/game/Rect.hpp>
#include <vix/game/Vec2.hpp>

namespace vix::game
{
  /**
   * @brief 2D camera description.
   *
   * Camera2D stores the information needed by a 2D renderer to transform world
   * coordinates into view coordinates.
   *
   * It is backend-independent and does not own any graphics resource.
   */
  struct Camera2D
  {
    /**
     * @brief Camera world position.
     */
    Vec2 position{Vec2::zero()};

    /**
     * @brief View size in world units.
     */
    Vec2 viewport{1280.0F, 720.0F};

    /**
     * @brief Camera zoom factor.
     *
     * A value of 1.0 means normal scale.
     */
    float zoom{1.0F};

    /**
     * @brief Camera rotation in radians.
     */
    float rotation{0.0F};

    /**
     * @brief Create a default camera.
     */
    constexpr Camera2D() noexcept = default;

    /**
     * @brief Create a camera from a viewport size.
     *
     * @param size Viewport size.
     */
    explicit constexpr Camera2D(Vec2 size) noexcept
        : viewport(size)
    {
    }

    /**
     * @brief Create a camera from width and height.
     *
     * @param width Viewport width.
     * @param height Viewport height.
     */
    constexpr Camera2D(
        float width,
        float height) noexcept
        : viewport(width, height)
    {
    }

    /**
     * @brief Return a default camera for a render target size.
     *
     * @param width Render width.
     * @param height Render height.
     * @return Camera.
     */
    [[nodiscard]] static constexpr Camera2D from_size(
        std::uint32_t width,
        std::uint32_t height) noexcept
    {
      return Camera2D{
          static_cast<float>(width),
          static_cast<float>(height)};
    }

    /**
     * @brief Set camera position.
     *
     * @param value New position.
     * @return Reference to this camera.
     */
    constexpr Camera2D &set_position(Vec2 value) noexcept
    {
      position = value;
      return *this;
    }

    /**
     * @brief Move the camera by an offset.
     *
     * @param offset Offset added to the current position.
     * @return Reference to this camera.
     */
    constexpr Camera2D &move(Vec2 offset) noexcept
    {
      position += offset;
      return *this;
    }

    /**
     * @brief Set viewport size.
     *
     * @param value New viewport size.
     * @return Reference to this camera.
     */
    constexpr Camera2D &set_viewport(Vec2 value) noexcept
    {
      viewport = value;
      return *this;
    }

    /**
     * @brief Set viewport size.
     *
     * @param width Viewport width.
     * @param height Viewport height.
     * @return Reference to this camera.
     */
    constexpr Camera2D &set_viewport(
        float width,
        float height) noexcept
    {
      viewport = Vec2{width, height};
      return *this;
    }

    /**
     * @brief Set zoom.
     *
     * If value is zero or negative, zoom is clamped to 1.0.
     *
     * @param value Zoom value.
     * @return Reference to this camera.
     */
    constexpr Camera2D &set_zoom(float value) noexcept
    {
      zoom = value <= 0.0F ? 1.0F : value;
      return *this;
    }

    /**
     * @brief Set rotation in radians.
     *
     * @param value Rotation in radians.
     * @return Reference to this camera.
     */
    constexpr Camera2D &set_rotation(float value) noexcept
    {
      rotation = value;
      return *this;
    }

    /**
     * @brief Rotate the camera by a delta in radians.
     *
     * @param delta Rotation delta.
     * @return Reference to this camera.
     */
    constexpr Camera2D &rotate(float delta) noexcept
    {
      rotation += delta;
      return *this;
    }

    /**
     * @brief Return the visible world rectangle.
     *
     * This uses position as the top-left corner of the view.
     *
     * @return View rectangle.
     */
    [[nodiscard]] constexpr Rect view_rect() const noexcept
    {
      const float safe_zoom = zoom <= 0.0F ? 1.0F : zoom;

      return Rect{
          position,
          Vec2{
              viewport.x / safe_zoom,
              viewport.y / safe_zoom}};
    }

    /**
     * @brief Return true if the viewport has a valid positive size.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      return viewport.x > 0.0F &&
             viewport.y > 0.0F &&
             zoom > 0.0F;
    }

    /**
     * @brief Convert a screen position to world position.
     *
     * This V2 version applies position and zoom only. Rotation-aware conversion
     * can be added when the renderer grows a full matrix system.
     *
     * @param screen Screen position.
     * @return World position.
     */
    [[nodiscard]] constexpr Vec2 screen_to_world(Vec2 screen) const noexcept
    {
      const float safe_zoom = zoom <= 0.0F ? 1.0F : zoom;

      return Vec2{
          position.x + screen.x / safe_zoom,
          position.y + screen.y / safe_zoom};
    }

    /**
     * @brief Convert a world position to screen position.
     *
     * This V2 version applies position and zoom only.
     *
     * @param world World position.
     * @return Screen position.
     */
    [[nodiscard]] constexpr Vec2 world_to_screen(Vec2 world) const noexcept
    {
      const float safe_zoom = zoom <= 0.0F ? 1.0F : zoom;

      return Vec2{
          (world.x - position.x) * safe_zoom,
          (world.y - position.y) * safe_zoom};
    }

    /**
     * @brief Compare two cameras.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Camera2D &lhs,
        const Camera2D &rhs) noexcept
    {
      return lhs.position == rhs.position &&
             lhs.viewport == rhs.viewport &&
             lhs.zoom == rhs.zoom &&
             lhs.rotation == rhs.rotation;
    }

    /**
     * @brief Compare two cameras.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Camera2D &lhs,
        const Camera2D &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

} // namespace vix::game

#endif // VIX_GAME_CAMERA_2D_HPP
