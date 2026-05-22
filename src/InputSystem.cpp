/**
 *
 *  @file InputSystem.cpp
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

#include <vix/game/InputSystem.hpp>

#include <utility>

namespace vix::game
{
  void InputSystem::begin_frame()
  {
    state_.begin_frame();
  }

  void InputSystem::handle_window_event(const WindowEvent &event)
  {
    switch (event.type)
    {
    case WindowEventType::KeyPressed:
      press_key(event.key);
      break;

    case WindowEventType::KeyReleased:
      release_key(event.key);
      break;

    case WindowEventType::ButtonPressed:
      press_button(event.button);
      break;

    case WindowEventType::ButtonReleased:
      release_button(event.button);
      break;

    case WindowEventType::PointerMoved:
      set_pointer_position(event.x, event.y);
      add_pointer_delta(event.dx, event.dy);
      break;

    case WindowEventType::WheelMoved:
      add_wheel_delta(event.dx, event.dy);
      break;

    case WindowEventType::Unknown:
    case WindowEventType::CloseRequested:
    case WindowEventType::Resized:
    case WindowEventType::FocusGained:
    case WindowEventType::FocusLost:
      break;
    }
  }

  void InputSystem::press_key(InputKey key)
  {
    state_.press_key(key);
  }

  void InputSystem::release_key(InputKey key)
  {
    state_.release_key(key);
  }

  void InputSystem::press_button(InputButton button)
  {
    state_.press_button(button);
  }

  void InputSystem::release_button(InputButton button)
  {
    state_.release_button(button);
  }

  void InputSystem::set_pointer_position(double x, double y) noexcept
  {
    state_.set_pointer_position(x, y);
  }

  void InputSystem::add_pointer_delta(double dx, double dy) noexcept
  {
    state_.add_pointer_delta(dx, dy);
  }

  void InputSystem::add_wheel_delta(double dx, double dy) noexcept
  {
    state_.add_wheel_delta(dx, dy);
  }

  GameBoolResult InputSystem::bind_key(
      std::string name,
      InputKey key)
  {
    return map_.bind_key(std::move(name), key);
  }

  GameBoolResult InputSystem::bind_button(
      std::string name,
      InputButton button)
  {
    return map_.bind_button(std::move(name), button);
  }

  GameBoolResult InputSystem::unbind(
      const std::string &name)
  {
    return map_.unbind(name);
  }

  bool InputSystem::key_down(InputKey key) const
  {
    return state_.key_down(key);
  }

  bool InputSystem::key_pressed(InputKey key) const
  {
    return state_.key_pressed(key);
  }

  bool InputSystem::key_released(InputKey key) const
  {
    return state_.key_released(key);
  }

  bool InputSystem::button_down(InputButton button) const
  {
    return state_.button_down(button);
  }

  bool InputSystem::button_pressed(InputButton button) const
  {
    return state_.button_pressed(button);
  }

  bool InputSystem::button_released(InputButton button) const
  {
    return state_.button_released(button);
  }

  bool InputSystem::action_down(const std::string &name) const
  {
    return map_.down(name, state_);
  }

  bool InputSystem::action_pressed(const std::string &name) const
  {
    return map_.pressed(name, state_);
  }

  bool InputSystem::action_released(const std::string &name) const
  {
    return map_.released(name, state_);
  }

  PointerPosition InputSystem::pointer_position() const noexcept
  {
    return state_.pointer_position();
  }

  PointerDelta InputSystem::pointer_delta() const noexcept
  {
    return state_.pointer_delta();
  }

  PointerDelta InputSystem::wheel_delta() const noexcept
  {
    return state_.wheel_delta();
  }

  InputState &InputSystem::state() noexcept
  {
    return state_;
  }

  const InputState &InputSystem::state() const noexcept
  {
    return state_;
  }

  InputMap &InputSystem::map() noexcept
  {
    return map_;
  }

  const InputMap &InputSystem::map() const noexcept
  {
    return map_;
  }

  void InputSystem::clear()
  {
    state_.clear();
    map_.clear();
  }

  void InputSystem::clear_state()
  {
    state_.clear();
  }

  void InputSystem::clear_bindings()
  {
    map_.clear();
  }

} // namespace vix::game
