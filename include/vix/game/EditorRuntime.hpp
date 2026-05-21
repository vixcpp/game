/**
 *
 *  @file EditorRuntime.hpp
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
#ifndef VIX_GAME_EDITOR_RUNTIME_HPP
#define VIX_GAME_EDITOR_RUNTIME_HPP

#include <vix/game/EditorContext.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/GameRuntime.hpp>

namespace vix::game
{
  /**
   * @brief Runtime coordinator for editor mode.
   *
   * EditorRuntime sits on top of GameRuntime and owns editor-only state through
   * EditorContext.
   *
   * It prepares the foundation for:
   * - editor tools
   * - scene inspection
   * - runtime editing
   * - play/edit mode switching
   * - future editor UI integration
   */
  class EditorRuntime
  {
  public:
    /**
     * @brief Construct an empty editor runtime.
     */
    EditorRuntime() = default;

    /**
     * @brief Construct an editor runtime from a game runtime.
     *
     * @param runtime Game runtime.
     */
    explicit EditorRuntime(GameRuntime &runtime) noexcept;

    EditorRuntime(const EditorRuntime &) = delete;
    EditorRuntime &operator=(const EditorRuntime &) = delete;

    /**
     * @brief Destroy the editor runtime.
     */
    ~EditorRuntime() = default;

    /**
     * @brief Attach to a game runtime.
     *
     * @param runtime Game runtime.
     * @return Reference to this runtime.
     */
    EditorRuntime &attach(GameRuntime &runtime) noexcept;

    /**
     * @brief Detach from the game runtime.
     */
    void detach() noexcept;

    /**
     * @brief Return true if attached to a game runtime.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Initialize editor runtime state.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init();

    /**
     * @brief Shut down editor runtime state.
     */
    void shutdown() noexcept;

    /**
     * @brief Return true if the editor runtime is initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Begin an editor frame.
     *
     * @param frame Current frame.
     */
    void begin_frame(const Frame &frame);

    /**
     * @brief Update editor systems.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame);

    /**
     * @brief Render editor systems.
     *
     * @param frame Current frame.
     */
    void render(const Frame &frame);

    /**
     * @brief End an editor frame.
     *
     * @param frame Current frame.
     */
    void end_frame(const Frame &frame);

    /**
     * @brief Enter editor edit mode.
     */
    void enter_edit_mode() noexcept;

    /**
     * @brief Enter editor play mode.
     */
    void enter_play_mode() noexcept;

    /**
     * @brief Return the editor context.
     */
    [[nodiscard]] EditorContext &context() noexcept;

    /**
     * @brief Return the editor context.
     */
    [[nodiscard]] const EditorContext &context() const noexcept;

    /**
     * @brief Return the game runtime.
     */
    [[nodiscard]] GameRuntime &game_runtime();

    /**
     * @brief Return the game runtime.
     */
    [[nodiscard]] const GameRuntime &game_runtime() const;

  private:
    /**
     * @brief Attached game runtime.
     */
    GameRuntime *runtime_{nullptr};

    /**
     * @brief Editor context.
     */
    EditorContext context_{};

    /**
     * @brief Whether init() completed.
     */
    bool initialized_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_EDITOR_RUNTIME_HPP
