/**
 *
 *  @file InputButton.hpp
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
#ifndef VIX_GAME_INPUT_BUTTON_HPP
#define VIX_GAME_INPUT_BUTTON_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Mouse or pointer button identifier used by the input module.
   *
   * InputButton is backend-independent.
   *
   * Window backends such as SDL, GLFW, or a custom platform layer should map
   * their native mouse button codes to this enum before updating InputState.
   */
  enum class InputButton : std::uint8_t
  {
    /**
     * @brief Unknown or unsupported button.
     */
    Unknown = 0,

    /**
     * @brief Primary mouse button, usually left click.
     */
    Left,

    /**
     * @brief Secondary mouse button, usually right click.
     */
    Right,

    /**
     * @brief Middle mouse button, usually wheel click.
     */
    Middle,

    /**
     * @brief Extra mouse button 1.
     */
    Button4,

    /**
     * @brief Extra mouse button 2.
     */
    Button5,

    /**
     * @brief Extra mouse button 3.
     */
    Button6,

    /**
     * @brief Extra mouse button 4.
     */
    Button7,

    /**
     * @brief Extra mouse button 5.
     */
    Button8
  };

  /**
   * @brief Convert an input button to a stable readable name.
   *
   * @param button Input button.
   * @return Static button name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      InputButton button) noexcept
  {
    switch (button)
    {
    case InputButton::Unknown:
      return "unknown";

    case InputButton::Left:
      return "left";

    case InputButton::Right:
      return "right";

    case InputButton::Middle:
      return "middle";

    case InputButton::Button4:
      return "button4";

    case InputButton::Button5:
      return "button5";

    case InputButton::Button6:
      return "button6";

    case InputButton::Button7:
      return "button7";

    case InputButton::Button8:
      return "button8";
    }

    return "unknown";
  }

  /**
   * @brief Check whether the button is valid.
   *
   * @param button Input button.
   * @return true if the button is not Unknown.
   */
  [[nodiscard]] inline constexpr bool is_valid_input_button(
      InputButton button) noexcept
  {
    return button != InputButton::Unknown;
  }

  /**
   * @brief Check whether the button is a primary pointer button.
   *
   * @param button Input button.
   * @return true if the button is Left.
   */
  [[nodiscard]] inline constexpr bool is_primary_button(
      InputButton button) noexcept
  {
    return button == InputButton::Left;
  }

  /**
   * @brief Check whether the button is a secondary pointer button.
   *
   * @param button Input button.
   * @return true if the button is Right.
   */
  [[nodiscard]] inline constexpr bool is_secondary_button(
      InputButton button) noexcept
  {
    return button == InputButton::Right;
  }

  /**
   * @brief Check whether the button is an extra pointer button.
   *
   * @param button Input button.
   * @return true if the button is Button4-Button8.
   */
  [[nodiscard]] inline constexpr bool is_extra_button(
      InputButton button) noexcept
  {
    return button >= InputButton::Button4 &&
           button <= InputButton::Button8;
  }

} // namespace vix::game

#endif // VIX_GAME_INPUT_BUTTON_HPP
