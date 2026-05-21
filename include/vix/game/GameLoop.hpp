/**
 *
 *  @file GameLoop.hpp
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
#ifndef VIX_GAME_GAME_LOOP_HPP
#define VIX_GAME_GAME_LOOP_HPP

#include <atomic>
#include <cstdint>
#include <functional>

#include <vix/game/Frame.hpp>
#include <vix/game/GameClock.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/TimeStep.hpp>
#include <vix/time/Duration.hpp>

namespace vix::game
{
  /**
   * @brief Main runtime loop for real-time applications.
   *
   * GameLoop owns the frame execution loop. It measures frame time, builds
   * Frame objects, invokes update callbacks, optionally runs fixed updates,
   * and can limit the frame rate based on TimeStep.
   *
   * GameLoop does not know about scenes, rendering, input, audio, or physics.
   * Those systems are attached by higher-level classes such as App.
   */
  class GameLoop
  {
  public:
    /**
     * @brief Callback invoked once per frame.
     */
    using UpdateCallback = std::function<void(const Frame &)>;

    /**
     * @brief Callback invoked for fixed simulation steps.
     */
    using FixedUpdateCallback = std::function<void(const Frame &)>;

    /**
     * @brief Callback used to decide whether the loop should keep running.
     */
    using ShouldContinueCallback = std::function<bool()>;

    /**
     * @brief Construct a game loop with default timing rules.
     */
    GameLoop();

    /**
     * @brief Construct a game loop with explicit timing rules.
     *
     * @param step Timing configuration.
     */
    explicit GameLoop(TimeStep step);

    GameLoop(const GameLoop &) = delete;
    GameLoop &operator=(const GameLoop &) = delete;

    /**
     * @brief Destroy the game loop.
     */
    ~GameLoop();

    /**
     * @brief Run the loop until stop() is called or should_continue returns false.
     *
     * @return true on normal completion, or an error on invalid state.
     */
    [[nodiscard]] GameBoolResult run();

    /**
     * @brief Request the loop to stop.
     */
    void stop() noexcept;

    /**
     * @brief Return true if the loop is currently running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Return true if stop() has been requested.
     */
    [[nodiscard]] bool stop_requested() const noexcept;

    /**
     * @brief Set the per-frame update callback.
     *
     * @param callback Callback invoked once per frame.
     */
    void set_update_callback(UpdateCallback callback);

    /**
     * @brief Set the fixed update callback.
     *
     * @param callback Callback invoked for each fixed simulation step.
     */
    void set_fixed_update_callback(FixedUpdateCallback callback);

    /**
     * @brief Set the continue predicate.
     *
     * If no callback is set, the loop continues until stop() is called.
     *
     * @param callback Predicate returning true to continue.
     */
    void set_should_continue_callback(ShouldContinueCallback callback);

    /**
     * @brief Set timing configuration.
     *
     * @param step New timing configuration.
     */
    void set_time_step(TimeStep step) noexcept;

    /**
     * @brief Return the current timing configuration.
     */
    [[nodiscard]] const TimeStep &time_step() const noexcept;

    /**
     * @brief Return the current frame index.
     */
    [[nodiscard]] std::uint64_t frame_index() const noexcept;

    /**
     * @brief Return the last produced frame.
     */
    [[nodiscard]] const Frame &last_frame() const noexcept;

  private:
    /**
     * @brief Execute one variable update callback.
     *
     * @param frame Current frame.
     */
    void run_update(const Frame &frame);

    /**
     * @brief Execute fixed update steps for the current frame.
     *
     * @param frame Current frame.
     * @param accumulator Accumulated simulation time.
     * @return Number of fixed steps executed.
     */
    std::uint32_t run_fixed_updates(
        const Frame &frame,
        vix::time::Duration &accumulator);

    /**
     * @brief Sleep if FPS limiting is enabled and the frame finished early.
     *
     * @param frame_started Time point when the frame started.
     */
    void limit_frame_rate(const GameClock::time_point &frame_started) const;

    /**
     * @brief Check whether the loop should continue.
     */
    [[nodiscard]] bool should_continue() const;

  private:
    /**
     * @brief Timing configuration.
     */
    TimeStep step_{};

    /**
     * @brief Clock used for loop timing.
     */
    GameClock clock_{};

    /**
     * @brief Per-frame update callback.
     */
    UpdateCallback update_{};

    /**
     * @brief Fixed update callback.
     */
    FixedUpdateCallback fixed_update_{};

    /**
     * @brief Optional predicate used to stop the loop externally.
     */
    ShouldContinueCallback should_continue_{};

    /**
     * @brief Whether the loop is currently running.
     */
    std::atomic<bool> running_{false};

    /**
     * @brief Whether stop() has been requested.
     */
    std::atomic<bool> stop_requested_{false};

    /**
     * @brief Current frame index.
     */
    std::uint64_t frame_index_{0};

    /**
     * @brief Last generated frame.
     */
    Frame last_frame_{};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_LOOP_HPP
