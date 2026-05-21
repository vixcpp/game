/**
 *
 *  @file GameClock.cpp
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

#include <vix/game/GameClock.hpp>

namespace vix::game
{
  void GameClock::start() noexcept
  {
    start_ = now();
    started_ = true;
  }

  void GameClock::reset() noexcept
  {
    start_ = time_point{};
    started_ = false;
  }

  bool GameClock::started() const noexcept
  {
    return started_;
  }

  GameClock::time_point GameClock::now() noexcept
  {
    return vix::time::SteadyClock::now_chrono();
  }

  vix::time::Timestamp GameClock::timestamp() noexcept
  {
    return vix::time::SystemClock::now();
  }

  vix::time::Duration GameClock::elapsed() const noexcept
  {
    if (!started_)
    {
      return vix::time::Duration{};
    }

    return since(start_);
  }

  vix::time::Duration GameClock::since(const time_point &point) noexcept
  {
    return vix::time::SteadyClock::since(point);
  }

  const GameClock::time_point &GameClock::start_time() const noexcept
  {
    return start_;
  }

} // namespace vix::game
