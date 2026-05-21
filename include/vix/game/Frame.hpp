/**
 *
 *  @file Frame.hpp
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
#ifndef VIX_GAME_FRAME_HPP
#define VIX_GAME_FRAME_HPP

#include <cstdint>

#include <vix/time/Duration.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::game
{
  /**
   * @brief Runtime information for a single game frame.
   *
   * Frame is passed to update callbacks, scenes, systems, and game loop
   * observers. It contains the frame index, the time elapsed since the previous
   * frame, the total elapsed runtime, and the wall-clock timestamp at which the
   * frame started.
   */
  struct Frame
  {
    /**
     * @brief Sequential frame index.
     *
     * Starts at 0 for the first frame.
     */
    std::uint64_t index{0};

    /**
     * @brief Time elapsed since the previous frame.
     */
    vix::time::Duration delta{};

    /**
     * @brief Total elapsed time since the game loop started.
     */
    vix::time::Duration elapsed{};

    /**
     * @brief Wall-clock timestamp when this frame started.
     */
    vix::time::Timestamp started_at{};

    /**
     * @brief Fixed update count executed during this frame.
     *
     * This is useful when a frame performs multiple fixed simulation steps
     * to catch up with accumulated time.
     */
    std::uint32_t fixed_steps{0};

    /**
     * @brief Check whether this is the first frame.
     *
     * @return true if index == 0.
     */
    [[nodiscard]] constexpr bool first() const noexcept
    {
      return index == 0;
    }

    /**
     * @brief Return frame delta time in milliseconds.
     *
     * @return Delta time truncated to milliseconds.
     */
    [[nodiscard]] constexpr vix::time::Duration::rep delta_ms() const noexcept
    {
      return delta.count_ms();
    }

    /**
     * @brief Return elapsed time in milliseconds.
     *
     * @return Elapsed time truncated to milliseconds.
     */
    [[nodiscard]] constexpr vix::time::Duration::rep elapsed_ms() const noexcept
    {
      return elapsed.count_ms();
    }
  };

} // namespace vix::game

#endif // VIX_GAME_FRAME_HPP
