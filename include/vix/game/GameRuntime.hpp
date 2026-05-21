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

namespace vix::game
{
  class App;

  /**
   * @brief Runtime coordinator for game applications.
   *
   * GameRuntime owns the high-level runtime flow around GameContext.
   *
   * It is responsible for:
   * - attaching to an App
   * - preparing per-frame systems
   * - forwarding update and fixed update phases
   * - preparing future editor, scripting, audio, and physics integrations
   */
  class GameRuntime
  {
  public:
    /**
     * @brief Construct an empty runtime.
     */
    GameRuntime() = default;

    /**
     * @brief Construct a runtime attached to an app.
     *
     * @param app Runtime app.
     */
    explicit GameRuntime(App &app) noexcept;

    GameRuntime(const GameRuntime &) = delete;
    GameRuntime &operator=(const GameRuntime &) = delete;

    /**
     * @brief Destroy the runtime.
     */
    ~GameRuntime() = default;

    /**
     * @brief Attach the runtime to an app.
     *
     * @param app Runtime app.
     * @return Reference to this runtime.
     */
    GameRuntime &attach(App &app) noexcept;

    /**
     * @brief Detach the runtime from its app.
     */
    void detach() noexcept;

    /**
     * @brief Return true if the runtime is attached to an app.
     */
    [[nodiscard]] bool attached() const noexcept;

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
     * @brief Return the runtime context.
     */
    [[nodiscard]] GameContext &context() noexcept;

    /**
     * @brief Return the runtime context.
     */
    [[nodiscard]] const GameContext &context() const noexcept;

  private:
    /**
     * @brief Runtime context.
     */
    GameContext context_{};

    /**
     * @brief Whether init() completed.
     */
    bool initialized_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_RUNTIME_HPP
