/**
 *
 *  @file GameClock.hpp
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
#ifndef VIX_GAME_GAME_CLOCK_HPP
#define VIX_GAME_GAME_CLOCK_HPP

#include <vix/time/Clock.hpp>
#include <vix/time/Duration.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::game
{
  /**
   * @brief Monotonic clock used by the game loop.
   *
   * GameClock wraps vix::time::SteadyClock for elapsed-time measurement and
   * vix::time::SystemClock for wall-clock frame timestamps.
   *
   * The steady clock is used for frame delta and elapsed runtime because it is
   * monotonic. The system clock is used only for user-facing timestamps.
   */
  class GameClock
  {
  public:
    /**
     * @brief Internal steady time point type.
     */
    using time_point = vix::time::SteadyClock::chrono_tp;

    /**
     * @brief Construct a stopped game clock.
     */
    GameClock() noexcept = default;

    /**
     * @brief Start or restart the clock.
     */
    void start() noexcept;

    /**
     * @brief Reset the clock to a non-started state.
     */
    void reset() noexcept;

    /**
     * @brief Return true if the clock has been started.
     */
    [[nodiscard]] bool started() const noexcept;

    /**
     * @brief Return the current monotonic time point.
     */
    [[nodiscard]] static time_point now() noexcept;

    /**
     * @brief Return the current wall-clock timestamp.
     */
    [[nodiscard]] static vix::time::Timestamp timestamp() noexcept;

    /**
     * @brief Return elapsed time since start().
     */
    [[nodiscard]] vix::time::Duration elapsed() const noexcept;

    /**
     * @brief Return elapsed time since a given time point.
     *
     * @param point Start time point.
     * @return Elapsed duration.
     */
    [[nodiscard]] static vix::time::Duration since(
        const time_point &point) noexcept;

    /**
     * @brief Return the start time point.
     */
    [[nodiscard]] const time_point &start_time() const noexcept;

  private:
    /**
     * @brief Time point recorded when the clock was started.
     */
    time_point start_{};

    /**
     * @brief Whether start_ contains a valid start time.
     */
    bool started_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_CLOCK_HPP
