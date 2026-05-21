/**
 *
 *  @file Rect.hpp
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
#ifndef VIX_GAME_RECT_HPP
#define VIX_GAME_RECT_HPP

#include <vix/game/Vec2.hpp>

namespace vix::game
{
  /**
   * @brief 2D rectangle used by the rendering foundation.
   *
   * Rect stores a position and a size.
   *
   * It is backend-independent and can be used for:
   * - sprite bounds
   * - texture regions
   * - camera view rectangles
   * - UI bounds later
   */
  struct Rect
  {
    /**
     * @brief X position.
     */
    float x{0.0F};

    /**
     * @brief Y position.
     */
    float y{0.0F};

    /**
     * @brief Rectangle width.
     */
    float width{0.0F};

    /**
     * @brief Rectangle height.
     */
    float height{0.0F};

    /**
     * @brief Create an empty rectangle.
     */
    constexpr Rect() noexcept = default;

    /**
     * @brief Create a rectangle from coordinates and size.
     *
     * @param x_value X position.
     * @param y_value Y position.
     * @param width_value Width.
     * @param height_value Height.
     */
    constexpr Rect(
        float x_value,
        float y_value,
        float width_value,
        float height_value) noexcept
        : x(x_value),
          y(y_value),
          width(width_value),
          height(height_value)
    {
    }

    /**
     * @brief Create a rectangle from position and size.
     *
     * @param position Rectangle position.
     * @param size Rectangle size.
     */
    constexpr Rect(
        Vec2 position,
        Vec2 size) noexcept
        : x(position.x),
          y(position.y),
          width(size.x),
          height(size.y)
    {
    }

    /**
     * @brief Return an empty rectangle.
     */
    [[nodiscard]] static constexpr Rect empty() noexcept
    {
      return Rect{};
    }

    /**
     * @brief Return rectangle position.
     */
    [[nodiscard]] constexpr Vec2 position() const noexcept
    {
      return Vec2{x, y};
    }

    /**
     * @brief Return rectangle size.
     */
    [[nodiscard]] constexpr Vec2 size() const noexcept
    {
      return Vec2{width, height};
    }

    /**
     * @brief Return the left coordinate.
     */
    [[nodiscard]] constexpr float left() const noexcept
    {
      return x;
    }

    /**
     * @brief Return the right coordinate.
     */
    [[nodiscard]] constexpr float right() const noexcept
    {
      return x + width;
    }

    /**
     * @brief Return the top coordinate.
     */
    [[nodiscard]] constexpr float top() const noexcept
    {
      return y;
    }

    /**
     * @brief Return the bottom coordinate.
     */
    [[nodiscard]] constexpr float bottom() const noexcept
    {
      return y + height;
    }

    /**
     * @brief Return true if width or height is zero or negative.
     */
    [[nodiscard]] constexpr bool empty_value() const noexcept
    {
      return width <= 0.0F || height <= 0.0F;
    }

    /**
     * @brief Return true if the rectangle has positive width and height.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      return width > 0.0F && height > 0.0F;
    }

    /**
     * @brief Return true if a point is inside the rectangle.
     *
     * @param point Point to test.
     * @return true if the point is inside.
     */
    [[nodiscard]] constexpr bool contains(Vec2 point) const noexcept
    {
      return point.x >= left() &&
             point.x <= right() &&
             point.y >= top() &&
             point.y <= bottom();
    }

    /**
     * @brief Return true if two rectangles overlap.
     *
     * @param other Other rectangle.
     * @return true if rectangles overlap.
     */
    [[nodiscard]] constexpr bool intersects(const Rect &other) const noexcept
    {
      if (empty_value() || other.empty_value())
      {
        return false;
      }

      return left() < other.right() &&
             right() > other.left() &&
             top() < other.bottom() &&
             bottom() > other.top();
    }

    /**
     * @brief Move the rectangle by an offset.
     *
     * @param offset Offset vector.
     * @return Reference to this rectangle.
     */
    constexpr Rect &translate(Vec2 offset) noexcept
    {
      x += offset.x;
      y += offset.y;
      return *this;
    }

    /**
     * @brief Set rectangle position.
     *
     * @param value New position.
     * @return Reference to this rectangle.
     */
    constexpr Rect &set_position(Vec2 value) noexcept
    {
      x = value.x;
      y = value.y;
      return *this;
    }

    /**
     * @brief Set rectangle size.
     *
     * @param value New size.
     * @return Reference to this rectangle.
     */
    constexpr Rect &set_size(Vec2 value) noexcept
    {
      width = value.x;
      height = value.y;
      return *this;
    }

    /**
     * @brief Compare two rectangles.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Rect &lhs,
        const Rect &rhs) noexcept
    {
      return lhs.x == rhs.x &&
             lhs.y == rhs.y &&
             lhs.width == rhs.width &&
             lhs.height == rhs.height;
    }

    /**
     * @brief Compare two rectangles.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Rect &lhs,
        const Rect &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

} // namespace vix::game

#endif // VIX_GAME_RECT_HPP
