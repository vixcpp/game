/**
 *
 *  @file EditorContext.cpp
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

#include <vix/game/EditorContext.hpp>
#include <utility>

namespace vix::game
{
  EditorContext::EditorContext(GameContext &context) noexcept
      : game_(&context),
        play_mode_(false),
        dirty_(false),
        selected_scene_(invalid_scene_id),
        active_tool_()
  {
  }

  EditorContext &EditorContext::attach(GameContext &context) noexcept
  {
    game_ = &context;
    return *this;
  }

  void EditorContext::detach() noexcept
  {
    clear();
    game_ = nullptr;
  }

  bool EditorContext::attached() const noexcept
  {
    return game_ != nullptr;
  }

  GameContext &EditorContext::game()
  {
    return *game_;
  }

  const GameContext &EditorContext::game() const
  {
    return *game_;
  }

  void EditorContext::enter_edit_mode() noexcept
  {
    play_mode_ = false;
  }

  void EditorContext::enter_play_mode() noexcept
  {
    play_mode_ = true;
  }

  bool EditorContext::play_mode() const noexcept
  {
    return play_mode_;
  }

  bool EditorContext::edit_mode() const noexcept
  {
    return !play_mode_;
  }

  void EditorContext::select_scene(SceneId id) noexcept
  {
    selected_scene_ = id;
  }

  void EditorContext::clear_selected_scene() noexcept
  {
    selected_scene_ = invalid_scene_id;
  }

  SceneId EditorContext::selected_scene() const noexcept
  {
    return selected_scene_;
  }

  bool EditorContext::has_selected_scene() const noexcept
  {
    return is_valid_scene_id(selected_scene_);
  }

  EditorContext &EditorContext::set_active_tool(std::string name)
  {
    active_tool_ = std::move(name);
    return *this;
  }

  const std::string &EditorContext::active_tool() const noexcept
  {
    return active_tool_;
  }

  bool EditorContext::has_active_tool() const noexcept
  {
    return !active_tool_.empty();
  }

  void EditorContext::mark_dirty() noexcept
  {
    dirty_ = true;
  }

  void EditorContext::clear_dirty() noexcept
  {
    dirty_ = false;
  }

  bool EditorContext::dirty() const noexcept
  {
    return dirty_;
  }

  void EditorContext::clear() noexcept
  {
    play_mode_ = false;
    dirty_ = false;
    selected_scene_ = invalid_scene_id;
    active_tool_.clear();
  }

} // namespace vix::game
