/**
 *
 *  @file TimeStep.hpp
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
#ifndef VIX_GAME_TIME_STEP_HPP
#define VIX_GAME_TIME_STEP_HPP

#include <cstdint>

#include <vix/time/Duration.hpp>

namespace vix::game
{
  /**
   * @brief Timing configuration for the game loop.
   *
   * TimeStep defines how the game loop advances time:
   * - target_fps controls the desired frame rate
   * - fixed_delta controls deterministic fixed update steps
   * - max_frame_delta clamps unusually large frame gaps
   *
   * This structure does not sleep or schedule by itself. It only stores
   * timing rules used by GameLoop.
   */
  struct TimeStep
  {
    /**
     * @brief Desired frame rate.
     *
     * A value of 60 means the game loop targets about 16.66 ms per frame.
     */
    std::uint32_t target_fps{60};

    /**
     * @brief Fixed simulation step duration.
     *
     * Default is 16 ms, close to 60 Hz simulation.
     */
    vix::time::Duration fixed_delta{vix::time::Duration::milliseconds(16)};

    /**
     * @brief Maximum accepted frame delta.
     *
     * Large deltas can happen after debugger pauses, stalls, or system sleep.
     * Clamping avoids unstable simulation jumps.
     */
    vix::time::Duration max_frame_delta{vix::time::Duration::milliseconds(250)};

    /**
     * @brief Whether the loop should try to respect target_fps.
     */
    bool limit_fps{true};

    /**
     * @brief Whether fixed update simulation is enabled.
     */
    bool fixed_update{true};

    /**
     * @brief Create a default timestep.
     *
     * @return Default TimeStep.
     */
    [[nodiscard]] static constexpr TimeStep defaults() noexcept
    {
      return TimeStep{};
    }

    /**
     * @brief Create a timestep from a target FPS.
     *
     * @param fps Target frames per second.
     * @return Configured TimeStep.
     */
    [[nodiscard]] static constexpr TimeStep from_fps(std::uint32_t fps) noexcept
    {
      TimeStep step;
      step.set_target_fps(fps);
      return step;
    }

    /**
     * @brief Return the target frame duration.
     *
     * @return Duration of one target frame.
     */
    [[nodiscard]] constexpr vix::time::Duration target_frame_duration() const noexcept
    {
      if (target_fps == 0)
      {
        return vix::time::Duration::milliseconds(0);
      }

      return vix::time::Duration::nanoseconds(
          1'000'000'000LL / static_cast<vix::time::Duration::rep>(target_fps));
    }

    /**
     * @brief Set the target FPS.
     *
     * If fps is 0, FPS limiting is disabled.
     *
     * @param fps New target FPS.
     * @return Reference to this object.
     */
    constexpr TimeStep &set_target_fps(std::uint32_t fps) noexcept
    {
      target_fps = fps;
      limit_fps = fps != 0;
      return *this;
    }

    /**
     * @brief Set the fixed update delta.
     *
     * @param value Fixed simulation step duration.
     * @return Reference to this object.
     */
    constexpr TimeStep &set_fixed_delta(vix::time::Duration value) noexcept
    {
      fixed_delta = value;
      return *this;
    }

    /**
     * @brief Set the maximum frame delta.
     *
     * @param value Maximum accepted frame delta.
     * @return Reference to this object.
     */
    constexpr TimeStep &set_max_frame_delta(vix::time::Duration value) noexcept
    {
      max_frame_delta = value;
      return *this;
    }

    /**
     * @brief Clamp a frame delta according to max_frame_delta.
     *
     * @param value Raw frame delta.
     * @return Clamped frame delta.
     */
    [[nodiscard]] constexpr vix::time::Duration clamp_delta(
        vix::time::Duration value) const noexcept
    {
      if (max_frame_delta.is_zero())
      {
        return value;
      }

      if (value > max_frame_delta)
      {
        return max_frame_delta;
      }

      return value;
    }
  };

} // namespace vix::game

#endif // VIX_GAME_TIME_STEP_HPP
