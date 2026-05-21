/**
 *
 *  @file Vec2.hpp
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
#ifndef VIX_GAME_VEC2_HPP
#define VIX_GAME_VEC2_HPP

namespace vix::game
{
  /**
   * @brief 2D vector used by the game 2D rendering foundation.
   *
   * Vec2 is intentionally small and backend-independent.
   *
   * It is used for:
   * - positions
   * - sizes
   * - scales
   * - offsets
   * - movement deltas
   */
  struct Vec2
  {
    /**
     * @brief X coordinate.
     */
    float x{0.0F};

    /**
     * @brief Y coordinate.
     */
    float y{0.0F};

    /**
     * @brief Create a zero vector.
     */
    constexpr Vec2() noexcept = default;

    /**
     * @brief Create a vector from two coordinates.
     *
     * @param x_value X coordinate.
     * @param y_value Y coordinate.
     */
    constexpr Vec2(
        float x_value,
        float y_value) noexcept
        : x(x_value),
          y(y_value)
    {
    }

    /**
     * @brief Return a zero vector.
     */
    [[nodiscard]] static constexpr Vec2 zero() noexcept
    {
      return Vec2{0.0F, 0.0F};
    }

    /**
     * @brief Return a one vector.
     */
    [[nodiscard]] static constexpr Vec2 one() noexcept
    {
      return Vec2{1.0F, 1.0F};
    }

    /**
     * @brief Return a unit X vector.
     */
    [[nodiscard]] static constexpr Vec2 unit_x() noexcept
    {
      return Vec2{1.0F, 0.0F};
    }

    /**
     * @brief Return a unit Y vector.
     */
    [[nodiscard]] static constexpr Vec2 unit_y() noexcept
    {
      return Vec2{0.0F, 1.0F};
    }

    /**
     * @brief Return true if both coordinates are zero.
     */
    [[nodiscard]] constexpr bool zero_value() const noexcept
    {
      return x == 0.0F && y == 0.0F;
    }

    /**
     * @brief Add another vector.
     *
     * @param other Vector to add.
     * @return Reference to this vector.
     */
    constexpr Vec2 &operator+=(const Vec2 &other) noexcept
    {
      x += other.x;
      y += other.y;
      return *this;
    }

    /**
     * @brief Subtract another vector.
     *
     * @param other Vector to subtract.
     * @return Reference to this vector.
     */
    constexpr Vec2 &operator-=(const Vec2 &other) noexcept
    {
      x -= other.x;
      y -= other.y;
      return *this;
    }

    /**
     * @brief Multiply by a scalar.
     *
     * @param scalar Scalar value.
     * @return Reference to this vector.
     */
    constexpr Vec2 &operator*=(float scalar) noexcept
    {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    /**
     * @brief Divide by a scalar.
     *
     * If scalar is zero, the vector is left unchanged.
     *
     * @param scalar Scalar value.
     * @return Reference to this vector.
     */
    constexpr Vec2 &operator/=(float scalar) noexcept
    {
      if (scalar == 0.0F)
      {
        return *this;
      }

      x /= scalar;
      y /= scalar;
      return *this;
    }

    /**
     * @brief Compare two vectors.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Vec2 &lhs,
        const Vec2 &rhs) noexcept
    {
      return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    /**
     * @brief Compare two vectors.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Vec2 &lhs,
        const Vec2 &rhs) noexcept
    {
      return !(lhs == rhs);
    }

    /**
     * @brief Add two vectors.
     */
    [[nodiscard]] friend constexpr Vec2 operator+(
        Vec2 lhs,
        const Vec2 &rhs) noexcept
    {
      lhs += rhs;
      return lhs;
    }

    /**
     * @brief Subtract two vectors.
     */
    [[nodiscard]] friend constexpr Vec2 operator-(
        Vec2 lhs,
        const Vec2 &rhs) noexcept
    {
      lhs -= rhs;
      return lhs;
    }

    /**
     * @brief Multiply a vector by a scalar.
     */
    [[nodiscard]] friend constexpr Vec2 operator*(
        Vec2 value,
        float scalar) noexcept
    {
      value *= scalar;
      return value;
    }

    /**
     * @brief Multiply a scalar by a vector.
     */
    [[nodiscard]] friend constexpr Vec2 operator*(
        float scalar,
        Vec2 value) noexcept
    {
      value *= scalar;
      return value;
    }

    /**
     * @brief Divide a vector by a scalar.
     */
    [[nodiscard]] friend constexpr Vec2 operator/(
        Vec2 value,
        float scalar) noexcept
    {
      value /= scalar;
      return value;
    }
  };

} // namespace vix::game

#endif // VIX_GAME_VEC2_HPP
