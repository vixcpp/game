/**
 *
 *  @file InputMap.hpp
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
#ifndef VIX_GAME_INPUT_MAP_HPP
#define VIX_GAME_INPUT_MAP_HPP

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/game/InputAction.hpp>
#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>
#include <vix/game/InputState.hpp>

namespace vix::game
{
  /**
   * @brief Maps named gameplay actions to backend-independent input bindings.
   *
   * InputMap is used to query actions instead of raw keys or buttons.
   *
   * Example:
   *
   *   input_map.bind_key("jump", InputKey::Space);
   *   input_map.pressed("jump", input_state);
   */
  class InputMap
  {
  public:
    /**
     * @brief Construct an empty input map.
     */
    InputMap() = default;

    /**
     * @brief Add or replace an action.
     *
     * @param action Input action.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult bind(InputAction action);

    /**
     * @brief Bind an action name to a keyboard key.
     *
     * @param name Action name.
     * @param key Input key.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult bind_key(
        std::string name,
        InputKey key);

    /**
     * @brief Bind an action name to a pointer button.
     *
     * @param name Action name.
     * @param button Input button.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult bind_button(
        std::string name,
        InputButton button);

    /**
     * @brief Remove an action by name.
     *
     * @param name Action name.
     * @return true if the action existed and was removed.
     */
    [[nodiscard]] GameBoolResult unbind(const std::string &name);

    /**
     * @brief Return true if an action exists.
     *
     * @param name Action name.
     * @return true if the action exists.
     */
    [[nodiscard]] bool contains(const std::string &name) const;

    /**
     * @brief Return an action by name.
     *
     * @param name Action name.
     * @return Input action pointer, or nullptr if missing.
     */
    [[nodiscard]] InputAction *get(const std::string &name);

    /**
     * @brief Return an action by name.
     *
     * @param name Action name.
     * @return Input action pointer, or nullptr if missing.
     */
    [[nodiscard]] const InputAction *get(const std::string &name) const;

    /**
     * @brief Return true if an action is currently held down.
     *
     * @param name Action name.
     * @param state Input state.
     * @return true if the action binding is down.
     */
    [[nodiscard]] bool down(
        const std::string &name,
        const InputState &state) const;

    /**
     * @brief Return true if an action was pressed during this frame.
     *
     * @param name Action name.
     * @param state Input state.
     * @return true if the action binding transitioned from up to down.
     */
    [[nodiscard]] bool pressed(
        const std::string &name,
        const InputState &state) const;

    /**
     * @brief Return true if an action was released during this frame.
     *
     * @param name Action name.
     * @param state Input state.
     * @return true if the action binding transitioned from down to up.
     */
    [[nodiscard]] bool released(
        const std::string &name,
        const InputState &state) const;

    /**
     * @brief Return all action names.
     */
    [[nodiscard]] std::vector<std::string> names() const;

    /**
     * @brief Return all actions.
     */
    [[nodiscard]] std::vector<InputAction> actions() const;

    /**
     * @brief Return the number of actions.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Return true if no action is registered.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Remove all actions.
     */
    void clear();

  private:
    /**
     * @brief Return true if a binding is currently held down.
     *
     * @param binding Input binding.
     * @param state Input state.
     * @return true if down.
     */
    [[nodiscard]] static bool binding_down(
        const InputBinding &binding,
        const InputState &state);

    /**
     * @brief Return true if a binding was pressed during this frame.
     *
     * @param binding Input binding.
     * @param state Input state.
     * @return true if pressed.
     */
    [[nodiscard]] static bool binding_pressed(
        const InputBinding &binding,
        const InputState &state);

    /**
     * @brief Return true if a binding was released during this frame.
     *
     * @param binding Input binding.
     * @param state Input state.
     * @return true if released.
     */
    [[nodiscard]] static bool binding_released(
        const InputBinding &binding,
        const InputState &state);

  private:
    /**
     * @brief Registered actions by name.
     */
    std::unordered_map<std::string, InputAction> actions_{};
  };

} // namespace vix::game

#endif // VIX_GAME_INPUT_MAP_HPP
