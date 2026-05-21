/**
 *
 *  @file Color.hpp
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
#ifndef VIX_GAME_COLOR_HPP
#define VIX_GAME_COLOR_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief RGBA color represented with normalized floating-point channels.
   *
   * Each channel is expected to be in the range [0.0, 1.0].
   *
   * Color is backend-independent and can be converted later by render backends
   * to the format required by OpenGL, Vulkan, DirectX, Metal, software renderers,
   * or image buffers.
   */
  struct Color
  {
    /**
     * @brief Red channel.
     */
    float r{1.0F};

    /**
     * @brief Green channel.
     */
    float g{1.0F};

    /**
     * @brief Blue channel.
     */
    float b{1.0F};

    /**
     * @brief Alpha channel.
     */
    float a{1.0F};

    /**
     * @brief Create a default white color.
     */
    constexpr Color() noexcept = default;

    /**
     * @brief Create a color from normalized channels.
     *
     * @param red Red channel.
     * @param green Green channel.
     * @param blue Blue channel.
     * @param alpha Alpha channel.
     */
    constexpr Color(
        float red,
        float green,
        float blue,
        float alpha = 1.0F) noexcept
        : r(red),
          g(green),
          b(blue),
          a(alpha)
    {
    }

    /**
     * @brief Return white.
     */
    [[nodiscard]] static constexpr Color white() noexcept
    {
      return Color{1.0F, 1.0F, 1.0F, 1.0F};
    }

    /**
     * @brief Return black.
     */
    [[nodiscard]] static constexpr Color black() noexcept
    {
      return Color{0.0F, 0.0F, 0.0F, 1.0F};
    }

    /**
     * @brief Return transparent black.
     */
    [[nodiscard]] static constexpr Color transparent() noexcept
    {
      return Color{0.0F, 0.0F, 0.0F, 0.0F};
    }

    /**
     * @brief Return red.
     */
    [[nodiscard]] static constexpr Color red() noexcept
    {
      return Color{1.0F, 0.0F, 0.0F, 1.0F};
    }

    /**
     * @brief Return green.
     */
    [[nodiscard]] static constexpr Color green() noexcept
    {
      return Color{0.0F, 1.0F, 0.0F, 1.0F};
    }

    /**
     * @brief Return blue.
     */
    [[nodiscard]] static constexpr Color blue() noexcept
    {
      return Color{0.0F, 0.0F, 1.0F, 1.0F};
    }

    /**
     * @brief Build a color from 8-bit RGBA channels.
     *
     * @param red Red channel in [0, 255].
     * @param green Green channel in [0, 255].
     * @param blue Blue channel in [0, 255].
     * @param alpha Alpha channel in [0, 255].
     * @return Normalized color.
     */
    [[nodiscard]] static constexpr Color from_rgba8(
        std::uint8_t red,
        std::uint8_t green,
        std::uint8_t blue,
        std::uint8_t alpha = 255) noexcept
    {
      return Color{
          static_cast<float>(red) / 255.0F,
          static_cast<float>(green) / 255.0F,
          static_cast<float>(blue) / 255.0F,
          static_cast<float>(alpha) / 255.0F};
    }

    /**
     * @brief Clamp all channels to [0.0, 1.0].
     *
     * @return Clamped color.
     */
    [[nodiscard]] constexpr Color clamped() const noexcept
    {
      return Color{
          clamp01(r),
          clamp01(g),
          clamp01(b),
          clamp01(a)};
    }

    /**
     * @brief Return true if alpha is zero or below.
     */
    [[nodiscard]] constexpr bool transparent_alpha() const noexcept
    {
      return a <= 0.0F;
    }

    /**
     * @brief Return true if alpha is one or above.
     */
    [[nodiscard]] constexpr bool opaque() const noexcept
    {
      return a >= 1.0F;
    }

    /**
     * @brief Compare two colors.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Color &lhs,
        const Color &rhs) noexcept
    {
      return lhs.r == rhs.r &&
             lhs.g == rhs.g &&
             lhs.b == rhs.b &&
             lhs.a == rhs.a;
    }

    /**
     * @brief Compare two colors.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Color &lhs,
        const Color &rhs) noexcept
    {
      return !(lhs == rhs);
    }

  private:
    /**
     * @brief Clamp one channel to [0.0, 1.0].
     *
     * @param value Channel value.
     * @return Clamped value.
     */
    [[nodiscard]] static constexpr float clamp01(float value) noexcept
    {
      if (value < 0.0F)
      {
        return 0.0F;
      }

      if (value > 1.0F)
      {
        return 1.0F;
      }

      return value;
    }
  };

} // namespace vix::game

#endif // VIX_GAME_COLOR_HPP
