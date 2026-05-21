/**
 *
 *  @file EditorContext.hpp
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
#ifndef VIX_GAME_EDITOR_CONTEXT_HPP
#define VIX_GAME_EDITOR_CONTEXT_HPP

#include <string>

#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/SceneId.hpp>

namespace vix::game
{
  /**
   * @brief Editor state and access layer.
   *
   * EditorContext is the editor-facing state container built on top of
   * GameContext.
   *
   * It does not own the game systems. It stores editor-specific state such as:
   * - selected scene
   * - selected entity
   * - play/edit mode
   * - dirty state
   * - active tool name
   */
  class EditorContext
  {
  public:
    /**
     * @brief Construct an empty editor context.
     */
    EditorContext() = default;

    /**
     * @brief Construct an editor context attached to a game context.
     *
     * @param context Game context.
     */
    explicit EditorContext(GameContext &context) noexcept;

    EditorContext(const EditorContext &) = delete;
    EditorContext &operator=(const EditorContext &) = delete;

    /**
     * @brief Destroy the editor context.
     */
    ~EditorContext() = default;

    /**
     * @brief Attach to a game context.
     *
     * @param context Game context.
     * @return Reference to this context.
     */
    EditorContext &attach(GameContext &context) noexcept;

    /**
     * @brief Detach from the game context.
     */
    void detach() noexcept;

    /**
     * @brief Return true if attached to a game context.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] GameContext &game();

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] const GameContext &game() const;

    /**
     * @brief Enter editor mode.
     */
    void enter_edit_mode() noexcept;

    /**
     * @brief Enter play mode.
     */
    void enter_play_mode() noexcept;

    /**
     * @brief Return true if the editor is in play mode.
     */
    [[nodiscard]] bool play_mode() const noexcept;

    /**
     * @brief Return true if the editor is in edit mode.
     */
    [[nodiscard]] bool edit_mode() const noexcept;

    /**
     * @brief Select a scene.
     *
     * @param id Scene id.
     */
    void select_scene(SceneId id) noexcept;

    /**
     * @brief Clear the selected scene.
     */
    void clear_selected_scene() noexcept;

    /**
     * @brief Return the selected scene id.
     */
    [[nodiscard]] SceneId selected_scene() const noexcept;

    /**
     * @brief Return true if a scene is selected.
     */
    [[nodiscard]] bool has_selected_scene() const noexcept;

    /**
     * @brief Set the active editor tool.
     *
     * @param name Tool name.
     * @return Reference to this context.
     */
    EditorContext &set_active_tool(std::string name);

    /**
     * @brief Return the active editor tool name.
     */
    [[nodiscard]] const std::string &active_tool() const noexcept;

    /**
     * @brief Return true if an active tool is set.
     */
    [[nodiscard]] bool has_active_tool() const noexcept;

    /**
     * @brief Mark the editor state as dirty.
     */
    void mark_dirty() noexcept;

    /**
     * @brief Clear the dirty flag.
     */
    void clear_dirty() noexcept;

    /**
     * @brief Return true if the editor state has unsaved changes.
     */
    [[nodiscard]] bool dirty() const noexcept;

    /**
     * @brief Reset editor-only state.
     */
    void clear() noexcept;

  private:
    /**
     * @brief Attached game context.
     */
    GameContext *game_{nullptr};

    /**
     * @brief Whether the editor is in play mode.
     */
    bool play_mode_{false};

    /**
     * @brief Whether editor state has unsaved changes.
     */
    bool dirty_{false};

    /**
     * @brief Selected scene id.
     */
    SceneId selected_scene_{invalid_scene_id};

    /**
     * @brief Active editor tool.
     */
    std::string active_tool_{};
  };

} // namespace vix::game

#endif // VIX_GAME_EDITOR_CONTEXT_HPP
