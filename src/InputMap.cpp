/**
 *
 *  @file InputMap.cpp
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

#include <vix/game/InputMap.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  GameBoolResult InputMap::bind(InputAction action)
  {
    if (!action.valid())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "input action must have a name and a valid binding");
    }

    actions_[action.name()] = std::move(action);
    return true;
  }

  GameBoolResult InputMap::bind_key(
      std::string name,
      InputKey key)
  {
    return bind(InputAction::key(std::move(name), key));
  }

  GameBoolResult InputMap::bind_button(
      std::string name,
      InputButton button)
  {
    return bind(InputAction::button(std::move(name), button));
  }

  GameBoolResult InputMap::unbind(const std::string &name)
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "input action name cannot be empty");
    }

    const auto erased = actions_.erase(name);
    if (erased == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "input action not found");
    }

    return true;
  }

  bool InputMap::contains(const std::string &name) const
  {
    return actions_.find(name) != actions_.end();
  }

  InputAction *InputMap::get(const std::string &name)
  {
    const auto it = actions_.find(name);
    if (it == actions_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  const InputAction *InputMap::get(const std::string &name) const
  {
    const auto it = actions_.find(name);
    if (it == actions_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  bool InputMap::down(
      const std::string &name,
      const InputState &state) const
  {
    const auto *action = get(name);
    if (action == nullptr)
    {
      return false;
    }

    return binding_down(action->binding(), state);
  }

  bool InputMap::pressed(
      const std::string &name,
      const InputState &state) const
  {
    const auto *action = get(name);
    if (action == nullptr)
    {
      return false;
    }

    return binding_pressed(action->binding(), state);
  }

  bool InputMap::released(
      const std::string &name,
      const InputState &state) const
  {
    const auto *action = get(name);
    if (action == nullptr)
    {
      return false;
    }

    return binding_released(action->binding(), state);
  }

  std::vector<std::string> InputMap::names() const
  {
    std::vector<std::string> out;
    out.reserve(actions_.size());

    for (const auto &[name, action] : actions_)
    {
      (void)action;
      out.push_back(name);
    }

    return out;
  }

  std::vector<InputAction> InputMap::actions() const
  {
    std::vector<InputAction> out;
    out.reserve(actions_.size());

    for (const auto &[name, action] : actions_)
    {
      (void)name;
      out.push_back(action);
    }

    return out;
  }

  std::size_t InputMap::size() const noexcept
  {
    return actions_.size();
  }

  bool InputMap::empty() const noexcept
  {
    return actions_.empty();
  }

  void InputMap::clear()
  {
    actions_.clear();
  }

  bool InputMap::binding_down(
      const InputBinding &binding,
      const InputState &state)
  {
    if (binding.is_key())
    {
      return state.key_down(binding.key);
    }

    if (binding.is_button())
    {
      return state.button_down(binding.button);
    }

    return false;
  }

  bool InputMap::binding_pressed(
      const InputBinding &binding,
      const InputState &state)
  {
    if (binding.is_key())
    {
      return state.key_pressed(binding.key);
    }

    if (binding.is_button())
    {
      return state.button_pressed(binding.button);
    }

    return false;
  }

  bool InputMap::binding_released(
      const InputBinding &binding,
      const InputState &state)
  {
    if (binding.is_key())
    {
      return state.key_released(binding.key);
    }

    if (binding.is_button())
    {
      return state.button_released(binding.button);
    }

    return false;
  }

} // namespace vix::game
