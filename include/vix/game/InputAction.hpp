/**
 *
 *  @file InputAction.hpp
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
#ifndef VIX_GAME_INPUT_ACTION_HPP
#define VIX_GAME_INPUT_ACTION_HPP

#include <string>
#include <utility>

#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>

namespace vix::game
{
  /**
   * @brief Type of input binding used by an input action.
   */
  enum class InputBindingType
  {
    /**
     * @brief No binding.
     */
    None = 0,

    /**
     * @brief Keyboard key binding.
     */
    Key,

    /**
     * @brief Mouse or pointer button binding.
     */
    Button
  };

  /**
   * @brief Convert an input binding type to a stable readable name.
   *
   * @param type Input binding type.
   * @return Static binding type name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      InputBindingType type) noexcept
  {
    switch (type)
    {
    case InputBindingType::None:
      return "none";

    case InputBindingType::Key:
      return "key";

    case InputBindingType::Button:
      return "button";
    }

    return "none";
  }

  /**
   * @brief Input binding attached to an input action.
   *
   * InputBinding is backend-independent.
   *
   * It can represent either:
   * - a keyboard key
   * - a mouse or pointer button
   */
  struct InputBinding
  {
    /**
     * @brief Binding type.
     */
    InputBindingType type{InputBindingType::None};

    /**
     * @brief Keyboard key.
     *
     * Used when type == InputBindingType::Key.
     */
    InputKey key{InputKey::Unknown};

    /**
     * @brief Mouse or pointer button.
     *
     * Used when type == InputBindingType::Button.
     */
    InputButton button{InputButton::Unknown};

    /**
     * @brief Create an empty binding.
     */
    constexpr InputBinding() noexcept = default;

    /**
     * @brief Create a key binding.
     *
     * @param value Input key.
     * @return Input binding.
     */
    [[nodiscard]] static constexpr InputBinding from_key(
        InputKey value) noexcept
    {
      InputBinding binding;
      binding.type = InputBindingType::Key;
      binding.key = value;
      return binding;
    }

    /**
     * @brief Create a button binding.
     *
     * @param value Input button.
     * @return Input binding.
     */
    [[nodiscard]] static constexpr InputBinding from_button(
        InputButton value) noexcept
    {
      InputBinding binding;
      binding.type = InputBindingType::Button;
      binding.button = value;
      return binding;
    }

    /**
     * @brief Return true if this binding is valid.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      if (type == InputBindingType::Key)
      {
        return key != InputKey::Unknown;
      }

      if (type == InputBindingType::Button)
      {
        return button != InputButton::Unknown;
      }

      return false;
    }

    /**
     * @brief Return true if this is a key binding.
     */
    [[nodiscard]] constexpr bool is_key() const noexcept
    {
      return type == InputBindingType::Key;
    }

    /**
     * @brief Return true if this is a button binding.
     */
    [[nodiscard]] constexpr bool is_button() const noexcept
    {
      return type == InputBindingType::Button;
    }

    /**
     * @brief Compare two input bindings.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const InputBinding &lhs,
        const InputBinding &rhs) noexcept
    {
      return lhs.type == rhs.type &&
             lhs.key == rhs.key &&
             lhs.button == rhs.button;
    }

    /**
     * @brief Compare two input bindings.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const InputBinding &lhs,
        const InputBinding &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

  /**
   * @brief Named input action.
   *
   * InputAction maps a human-readable action name to a backend-independent
   * input binding.
   *
   * Examples:
   * - "jump" -> Space
   * - "shoot" -> Left mouse button
   * - "pause" -> Escape
   */
  class InputAction
  {
  public:
    /**
     * @brief Construct an empty input action.
     */
    InputAction() = default;

    /**
     * @brief Construct an input action.
     *
     * @param name Action name.
     * @param binding Input binding.
     */
    InputAction(std::string name, InputBinding binding)
        : name_(std::move(name)),
          binding_(binding)
    {
    }

    /**
     * @brief Create an input action bound to a keyboard key.
     *
     * @param name Action name.
     * @param key Input key.
     * @return Input action.
     */
    [[nodiscard]] static InputAction key(
        std::string name,
        InputKey key)
    {
      return InputAction(
          std::move(name),
          InputBinding::from_key(key));
    }

    /**
     * @brief Create an input action bound to a mouse or pointer button.
     *
     * @param name Action name.
     * @param button Input button.
     * @return Input action.
     */
    [[nodiscard]] static InputAction button(
        std::string name,
        InputButton button)
    {
      return InputAction(
          std::move(name),
          InputBinding::from_button(button));
    }

    /**
     * @brief Return the action name.
     */
    [[nodiscard]] const std::string &name() const noexcept
    {
      return name_;
    }

    /**
     * @brief Set the action name.
     *
     * @param value New action name.
     * @return Reference to this action.
     */
    InputAction &set_name(std::string value)
    {
      name_ = std::move(value);
      return *this;
    }

    /**
     * @brief Return the action binding.
     */
    [[nodiscard]] const InputBinding &binding() const noexcept
    {
      return binding_;
    }

    /**
     * @brief Set the action binding.
     *
     * @param value New binding.
     * @return Reference to this action.
     */
    InputAction &set_binding(InputBinding value) noexcept
    {
      binding_ = value;
      return *this;
    }

    /**
     * @brief Return true if this action has a non-empty name and valid binding.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return !name_.empty() && binding_.valid();
    }

  private:
    /**
     * @brief Action name.
     */
    std::string name_{};

    /**
     * @brief Action binding.
     */
    InputBinding binding_{};
  };

} // namespace vix::game

#endif // VIX_GAME_INPUT_ACTION_HPP
