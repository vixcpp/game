/**
 *
 *  @file InputKey.hpp
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
#ifndef VIX_GAME_INPUT_KEY_HPP
#define VIX_GAME_INPUT_KEY_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Keyboard key identifier used by the input module.
   *
   * InputKey is backend-independent.
   *
   * Window backends such as SDL, GLFW, or a custom platform layer should map
   * their native key codes to this enum before updating InputState.
   */
  enum class InputKey : std::uint16_t
  {
    Unknown = 0,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    Escape,
    Tab,
    CapsLock,
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,
    Space,
    Enter,
    Backspace,

    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
  };

  /**
   * @brief Convert an input key to a stable readable name.
   *
   * @param key Input key.
   * @return Static key name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      InputKey key) noexcept
  {
    switch (key)
    {
    case InputKey::Unknown:
      return "unknown";

    case InputKey::A:
      return "a";
    case InputKey::B:
      return "b";
    case InputKey::C:
      return "c";
    case InputKey::D:
      return "d";
    case InputKey::E:
      return "e";
    case InputKey::F:
      return "f";
    case InputKey::G:
      return "g";
    case InputKey::H:
      return "h";
    case InputKey::I:
      return "i";
    case InputKey::J:
      return "j";
    case InputKey::K:
      return "k";
    case InputKey::L:
      return "l";
    case InputKey::M:
      return "m";
    case InputKey::N:
      return "n";
    case InputKey::O:
      return "o";
    case InputKey::P:
      return "p";
    case InputKey::Q:
      return "q";
    case InputKey::R:
      return "r";
    case InputKey::S:
      return "s";
    case InputKey::T:
      return "t";
    case InputKey::U:
      return "u";
    case InputKey::V:
      return "v";
    case InputKey::W:
      return "w";
    case InputKey::X:
      return "x";
    case InputKey::Y:
      return "y";
    case InputKey::Z:
      return "z";

    case InputKey::Num0:
      return "0";
    case InputKey::Num1:
      return "1";
    case InputKey::Num2:
      return "2";
    case InputKey::Num3:
      return "3";
    case InputKey::Num4:
      return "4";
    case InputKey::Num5:
      return "5";
    case InputKey::Num6:
      return "6";
    case InputKey::Num7:
      return "7";
    case InputKey::Num8:
      return "8";
    case InputKey::Num9:
      return "9";

    case InputKey::Escape:
      return "escape";
    case InputKey::Tab:
      return "tab";
    case InputKey::CapsLock:
      return "caps_lock";
    case InputKey::LeftShift:
      return "left_shift";
    case InputKey::RightShift:
      return "right_shift";
    case InputKey::LeftControl:
      return "left_control";
    case InputKey::RightControl:
      return "right_control";
    case InputKey::LeftAlt:
      return "left_alt";
    case InputKey::RightAlt:
      return "right_alt";
    case InputKey::Space:
      return "space";
    case InputKey::Enter:
      return "enter";
    case InputKey::Backspace:
      return "backspace";

    case InputKey::Insert:
      return "insert";
    case InputKey::Delete:
      return "delete";
    case InputKey::Home:
      return "home";
    case InputKey::End:
      return "end";
    case InputKey::PageUp:
      return "page_up";
    case InputKey::PageDown:
      return "page_down";

    case InputKey::ArrowLeft:
      return "arrow_left";
    case InputKey::ArrowRight:
      return "arrow_right";
    case InputKey::ArrowUp:
      return "arrow_up";
    case InputKey::ArrowDown:
      return "arrow_down";

    case InputKey::F1:
      return "f1";
    case InputKey::F2:
      return "f2";
    case InputKey::F3:
      return "f3";
    case InputKey::F4:
      return "f4";
    case InputKey::F5:
      return "f5";
    case InputKey::F6:
      return "f6";
    case InputKey::F7:
      return "f7";
    case InputKey::F8:
      return "f8";
    case InputKey::F9:
      return "f9";
    case InputKey::F10:
      return "f10";
    case InputKey::F11:
      return "f11";
    case InputKey::F12:
      return "f12";
    }

    return "unknown";
  }

  /**
   * @brief Check whether the key is a letter key.
   *
   * @param key Input key.
   * @return true if the key is A-Z.
   */
  [[nodiscard]] inline constexpr bool is_letter_key(
      InputKey key) noexcept
  {
    return key >= InputKey::A && key <= InputKey::Z;
  }

  /**
   * @brief Check whether the key is a number key.
   *
   * @param key Input key.
   * @return true if the key is 0-9.
   */
  [[nodiscard]] inline constexpr bool is_number_key(
      InputKey key) noexcept
  {
    return key >= InputKey::Num0 && key <= InputKey::Num9;
  }

  /**
   * @brief Check whether the key is a function key.
   *
   * @param key Input key.
   * @return true if the key is F1-F12.
   */
  [[nodiscard]] inline constexpr bool is_function_key(
      InputKey key) noexcept
  {
    return key >= InputKey::F1 && key <= InputKey::F12;
  }

  /**
   * @brief Check whether the key is a modifier key.
   *
   * @param key Input key.
   * @return true if the key is Shift, Control, or Alt.
   */
  [[nodiscard]] inline constexpr bool is_modifier_key(
      InputKey key) noexcept
  {
    return key == InputKey::LeftShift ||
           key == InputKey::RightShift ||
           key == InputKey::LeftControl ||
           key == InputKey::RightControl ||
           key == InputKey::LeftAlt ||
           key == InputKey::RightAlt;
  }

  /**
   * @brief Check whether the key is an arrow key.
   *
   * @param key Input key.
   * @return true if the key is an arrow key.
   */
  [[nodiscard]] inline constexpr bool is_arrow_key(
      InputKey key) noexcept
  {
    return key == InputKey::ArrowLeft ||
           key == InputKey::ArrowRight ||
           key == InputKey::ArrowUp ||
           key == InputKey::ArrowDown;
  }

} // namespace vix::game

#endif // VIX_GAME_INPUT_KEY_HPP
