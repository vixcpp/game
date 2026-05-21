/**
 *
 *  @file GameLoop.cpp
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

#include <vix/game/GameLoop.hpp>

#include <chrono>
#include <thread>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  GameLoop::GameLoop()
      : GameLoop(TimeStep{})
  {
  }

  GameLoop::GameLoop(TimeStep step)
      : step_(step),
        clock_(),
        update_(),
        fixed_update_(),
        should_continue_(),
        running_(false),
        stop_requested_(false),
        frame_index_(0),
        last_frame_()
  {
  }

  GameLoop::~GameLoop()
  {
    stop();
  }

  GameBoolResult GameLoop::run()
  {
    bool expected = false;
    if (!running_.compare_exchange_strong(expected, true))
    {
      return make_game_error(
          GameErrorCode::GameLoopAlreadyRunning,
          "game loop is already running");
    }

    stop_requested_.store(false);
    frame_index_ = 0;
    last_frame_ = Frame{};

    clock_.start();

    auto previous = GameClock::now();
    vix::time::Duration fixed_accumulator{};

    while (!stop_requested_.load() && should_continue())
    {
      const auto frame_started = GameClock::now();

      vix::time::Duration delta = GameClock::since(previous);
      previous = frame_started;
      delta = step_.clamp_delta(delta);

      fixed_accumulator += delta;

      Frame frame;
      frame.index = frame_index_;
      frame.delta = delta;
      frame.elapsed = clock_.elapsed();
      frame.started_at = GameClock::timestamp();

      frame.fixed_steps = run_fixed_updates(frame, fixed_accumulator);

      run_update(frame);

      last_frame_ = frame;
      ++frame_index_;

      limit_frame_rate(frame_started);
    }

    running_.store(false);
    stop_requested_.store(false);

    return true;
  }

  void GameLoop::stop() noexcept
  {
    stop_requested_.store(true);
  }

  bool GameLoop::running() const noexcept
  {
    return running_.load();
  }

  bool GameLoop::stop_requested() const noexcept
  {
    return stop_requested_.load();
  }

  void GameLoop::set_update_callback(UpdateCallback callback)
  {
    update_ = std::move(callback);
  }

  void GameLoop::set_fixed_update_callback(FixedUpdateCallback callback)
  {
    fixed_update_ = std::move(callback);
  }

  void GameLoop::set_should_continue_callback(ShouldContinueCallback callback)
  {
    should_continue_ = std::move(callback);
  }

  void GameLoop::set_time_step(TimeStep step) noexcept
  {
    step_ = step;
  }

  const TimeStep &GameLoop::time_step() const noexcept
  {
    return step_;
  }

  std::uint64_t GameLoop::frame_index() const noexcept
  {
    return frame_index_;
  }

  const Frame &GameLoop::last_frame() const noexcept
  {
    return last_frame_;
  }

  void GameLoop::run_update(const Frame &frame)
  {
    if (update_)
    {
      update_(frame);
    }
  }

  std::uint32_t GameLoop::run_fixed_updates(
      const Frame &frame,
      vix::time::Duration &accumulator)
  {
    if (!step_.fixed_update || !fixed_update_)
    {
      return 0;
    }

    if (step_.fixed_delta.is_zero())
    {
      return 0;
    }

    std::uint32_t steps = 0;

    while (accumulator >= step_.fixed_delta)
    {
      fixed_update_(frame);
      accumulator -= step_.fixed_delta;
      ++steps;
    }

    return steps;
  }

  void GameLoop::limit_frame_rate(
      const GameClock::time_point &frame_started) const
  {
    if (!step_.limit_fps)
    {
      return;
    }

    const auto target = step_.target_frame_duration();
    if (target.is_zero())
    {
      return;
    }

    const auto elapsed = GameClock::since(frame_started);
    if (elapsed >= target)
    {
      return;
    }

    const auto remaining = target - elapsed;
    std::this_thread::sleep_for(remaining.to_chrono());
  }

  bool GameLoop::should_continue() const
  {
    if (!should_continue_)
    {
      return true;
    }

    return should_continue_();
  }

} // namespace vix::game
