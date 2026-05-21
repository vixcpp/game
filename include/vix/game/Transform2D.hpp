/**
 *
 *  @file Transform2D.hpp
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
#ifndef VIX_GAME_TRANSFORM_2D_HPP
#define VIX_GAME_TRANSFORM_2D_HPP

#include <vix/game/Vec2.hpp>

namespace vix::game
{
  /**
   * @brief 2D transform component.
   *
   * Transform2D stores position, scale, rotation, origin, and draw depth.
   *
   * It is designed to be used as a plain ECS component:
   *
   *   registry.emplace_component<Transform2D>(entity.id());
   *
   * Rotation is expressed in radians.
   */
  struct Transform2D
  {
    /**
     * @brief World position.
     */
    Vec2 position{Vec2::zero()};

    /**
     * @brief Scale factor.
     */
    Vec2 scale{Vec2::one()};

    /**
     * @brief Local origin or pivot.
     */
    Vec2 origin{Vec2::zero()};

    /**
     * @brief Rotation in radians.
     */
    float rotation{0.0F};

    /**
     * @brief Draw depth.
     *
     * Lower values can be rendered first by 2D systems.
     */
    float depth{0.0F};

    /**
     * @brief Create an identity transform.
     */
    constexpr Transform2D() noexcept = default;

    /**
     * @brief Create a transform from position.
     *
     * @param value Position.
     */
    explicit constexpr Transform2D(Vec2 value) noexcept
        : position(value)
    {
    }

    /**
     * @brief Create a transform from position and scale.
     *
     * @param position_value Position.
     * @param scale_value Scale.
     */
    constexpr Transform2D(
        Vec2 position_value,
        Vec2 scale_value) noexcept
        : position(position_value),
          scale(scale_value)
    {
    }

    /**
     * @brief Return an identity transform.
     */
    [[nodiscard]] static constexpr Transform2D identity() noexcept
    {
      return Transform2D{};
    }

    /**
     * @brief Return a transform with a given position.
     *
     * @param value Position.
     * @return Transform.
     */
    [[nodiscard]] static constexpr Transform2D at(Vec2 value) noexcept
    {
      return Transform2D{value};
    }

    /**
     * @brief Set position.
     *
     * @param value New position.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_position(Vec2 value) noexcept
    {
      position = value;
      return *this;
    }

    /**
     * @brief Move by an offset.
     *
     * @param offset Offset to add to position.
     * @return Reference to this transform.
     */
    constexpr Transform2D &translate(Vec2 offset) noexcept
    {
      position += offset;
      return *this;
    }

    /**
     * @brief Set scale.
     *
     * @param value New scale.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_scale(Vec2 value) noexcept
    {
      scale = value;
      return *this;
    }

    /**
     * @brief Set uniform scale.
     *
     * @param value Uniform scale value.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_scale(float value) noexcept
    {
      scale = Vec2{value, value};
      return *this;
    }

    /**
     * @brief Set local origin.
     *
     * @param value New origin.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_origin(Vec2 value) noexcept
    {
      origin = value;
      return *this;
    }

    /**
     * @brief Set rotation in radians.
     *
     * @param value Rotation in radians.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_rotation(float value) noexcept
    {
      rotation = value;
      return *this;
    }

    /**
     * @brief Rotate by a delta in radians.
     *
     * @param delta Rotation delta in radians.
     * @return Reference to this transform.
     */
    constexpr Transform2D &rotate(float delta) noexcept
    {
      rotation += delta;
      return *this;
    }

    /**
     * @brief Set draw depth.
     *
     * @param value Draw depth.
     * @return Reference to this transform.
     */
    constexpr Transform2D &set_depth(float value) noexcept
    {
      depth = value;
      return *this;
    }

    /**
     * @brief Return true if the transform has identity values.
     */
    [[nodiscard]] constexpr bool identity_value() const noexcept
    {
      return position == Vec2::zero() &&
             scale == Vec2::one() &&
             origin == Vec2::zero() &&
             rotation == 0.0F &&
             depth == 0.0F;
    }

    /**
     * @brief Compare two transforms.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Transform2D &lhs,
        const Transform2D &rhs) noexcept
    {
      return lhs.position == rhs.position &&
             lhs.scale == rhs.scale &&
             lhs.origin == rhs.origin &&
             lhs.rotation == rhs.rotation &&
             lhs.depth == rhs.depth;
    }

    /**
     * @brief Compare two transforms.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Transform2D &lhs,
        const Transform2D &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

} // namespace vix::game

#endif // VIX_GAME_TRANSFORM_2D_HPP
