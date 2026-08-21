/**
 *
 *  @file GameRuntime.hpp
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
#ifndef VIX_GAME_GAME_RUNTIME_HPP
#define VIX_GAME_GAME_RUNTIME_HPP

#include <vix/game/Frame.hpp>
#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/RuntimeDiagnostics.hpp>

namespace vix::game
{
  class App;

  /**
   * @brief Runtime coordinator for game applications.
   *
   * GameRuntime is owned exclusively by App and owns the high-level runtime
   * flow around GameContext. It cannot be constructed or attached separately.
   *
   * It is responsible for:
   * - initializing runtime backends for its App
   * - preparing per-frame systems
   * - forwarding update and fixed update phases
   * - preparing future editor, scripting, audio, and physics integrations
   */
  class GameRuntime
  {
  public:
    GameRuntime(const GameRuntime &) = delete;
    GameRuntime &operator=(const GameRuntime &) = delete;

    /**
     * @brief Destroy the runtime.
     */
    ~GameRuntime() = default;

    /**
     * @brief Initialize runtime systems.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init();

    /**
     * @brief Shut down runtime systems.
     */
    void shutdown() noexcept;

    /**
     * @brief Return true if the runtime is initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Return true while App is executing this runtime's loop.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Begin a frame.
     *
     * @param frame Current frame.
     */
    void begin_frame(const Frame &frame);

    /**
     * @brief Update runtime systems.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame);

    /**
     * @brief Fixed update runtime systems.
     *
     * @param frame Current frame.
     */
    void fixed_update(const Frame &frame);

    /**
     * @brief Render runtime systems.
     *
     * @param frame Current frame.
     */
    void render(const Frame &frame);

    /**
     * @brief End a frame.
     *
     * @param frame Current frame.
     */
    void end_frame(const Frame &frame);

    /**
     * @brief Return the latest runtime diagnostics snapshot.
     */
    [[nodiscard]] RuntimeDiagnostics diagnostics() const;

    /**
     * @brief Return the runtime context.
     */
    [[nodiscard]] GameContext &context() noexcept;

    /**
     * @brief Return the runtime context.
     */
    [[nodiscard]] const GameContext &context() const noexcept;

  private:
    friend class App;

    /**
     * @brief Construct the single runtime owned by an App.
     */
    explicit GameRuntime(App &app) noexcept;

    /** @brief Mark frame processing as active for App::run(). */
    void begin_run() noexcept;

    /** @brief Mark frame processing as inactive after App::run(). */
    void end_run() noexcept;

    /**
     * @brief Runtime context.
     */
    GameContext context_;

    /**
     * @brief Last runtime diagnostics snapshot.
     */
    RuntimeDiagnostics diagnostics_{};

    /**
     * @brief Whether init() completed.
     */
    bool initialized_{false};

    /** Whether App is currently executing the loop. */
    bool running_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_RUNTIME_HPP
