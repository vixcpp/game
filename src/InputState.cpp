/**
 *
 *  @file InputState.cpp
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

#include <vix/game/InputState.hpp>

namespace vix::game
{
  void InputState::begin_frame()
  {
    previous_keys_ = current_keys_;
    previous_buttons_ = current_buttons_;

    pointer_delta_ = PointerDelta{};
    wheel_delta_ = PointerDelta{};
  }

  void InputState::press_key(InputKey key)
  {
    if (key == InputKey::Unknown)
    {
      return;
    }

    current_keys_.insert(key);
  }

  void InputState::release_key(InputKey key)
  {
    if (key == InputKey::Unknown)
    {
      return;
    }

    current_keys_.erase(key);
  }

  void InputState::press_button(InputButton button)
  {
    if (!is_valid_input_button(button))
    {
      return;
    }

    current_buttons_.insert(button);
  }

  void InputState::release_button(InputButton button)
  {
    if (!is_valid_input_button(button))
    {
      return;
    }

    current_buttons_.erase(button);
  }

  void InputState::set_pointer_position(double x, double y) noexcept
  {
    pointer_.x = x;
    pointer_.y = y;
  }

  void InputState::add_pointer_delta(double dx, double dy) noexcept
  {
    pointer_delta_.x += dx;
    pointer_delta_.y += dy;
  }

  void InputState::add_wheel_delta(double dx, double dy) noexcept
  {
    wheel_delta_.x += dx;
    wheel_delta_.y += dy;
  }

  bool InputState::key_down(InputKey key) const
  {
    return current_keys_.find(key) != current_keys_.end();
  }

  bool InputState::key_pressed(InputKey key) const
  {
    return current_keys_.find(key) != current_keys_.end() &&
           previous_keys_.find(key) == previous_keys_.end();
  }

  bool InputState::key_released(InputKey key) const
  {
    return current_keys_.find(key) == current_keys_.end() &&
           previous_keys_.find(key) != previous_keys_.end();
  }

  bool InputState::button_down(InputButton button) const
  {
    return current_buttons_.find(button) != current_buttons_.end();
  }

  bool InputState::button_pressed(InputButton button) const
  {
    return current_buttons_.find(button) != current_buttons_.end() &&
           previous_buttons_.find(button) == previous_buttons_.end();
  }

  bool InputState::button_released(InputButton button) const
  {
    return current_buttons_.find(button) == current_buttons_.end() &&
           previous_buttons_.find(button) != previous_buttons_.end();
  }

  PointerPosition InputState::pointer_position() const noexcept
  {
    return pointer_;
  }

  PointerDelta InputState::pointer_delta() const noexcept
  {
    return pointer_delta_;
  }

  PointerDelta InputState::wheel_delta() const noexcept
  {
    return wheel_delta_;
  }

  bool InputState::any_down() const noexcept
  {
    return !current_keys_.empty() || !current_buttons_.empty();
  }

  void InputState::clear()
  {
    current_keys_.clear();
    previous_keys_.clear();

    current_buttons_.clear();
    previous_buttons_.clear();

    pointer_ = PointerPosition{};
    pointer_delta_ = PointerDelta{};
    wheel_delta_ = PointerDelta{};
  }

} // namespace vix::game
