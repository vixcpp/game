/**
 *
 *  @file InputSystem.hpp
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
#ifndef VIX_GAME_INPUT_SYSTEM_HPP
#define VIX_GAME_INPUT_SYSTEM_HPP

#include <string>

#include <vix/game/GameResult.hpp>
#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>
#include <vix/game/InputMap.hpp>
#include <vix/game/InputState.hpp>
#include <vix/game/WindowEvent.hpp>

namespace vix::game
{
  /**
   * @brief High-level input facade for vix/game.
   *
   * InputSystem owns:
   * - the current input state
   * - the input action map
   *
   * Platform/window backends should feed raw input events into InputSystem
   * using press/release/move/wheel methods.
   *
   * Game code can then query either raw input:
   * - key_down()
   * - button_pressed()
   *
   * Or action-based input:
   * - action_down("jump")
   * - action_pressed("shoot")
   */
  class InputSystem
  {
  public:
    /**
     * @brief Construct an empty input system.
     */
    InputSystem() = default;

    InputSystem(const InputSystem &) = delete;
    InputSystem &operator=(const InputSystem &) = delete;

    /**
     * @brief Destroy the input system.
     */
    ~InputSystem() = default;

    /**
     * @brief Begin a new input frame.
     *
     * This snapshots the previous state and clears per-frame deltas.
     */
    void begin_frame();

    /**
     * @brief Apply one backend-independent window event to the input state.
     *
     * @param event Window event produced by a window backend.
     */
    void handle_window_event(const WindowEvent &event);

    /**
     * @brief Mark a key as pressed.
     *
     * @param key Input key.
     */
    void press_key(InputKey key);

    /**
     * @brief Mark a key as released.
     *
     * @param key Input key.
     */
    void release_key(InputKey key);

    /**
     * @brief Mark a pointer button as pressed.
     *
     * @param button Input button.
     */
    void press_button(InputButton button);

    /**
     * @brief Mark a pointer button as released.
     *
     * @param button Input button.
     */
    void release_button(InputButton button);

    /**
     * @brief Set the current pointer position.
     *
     * @param x Horizontal position.
     * @param y Vertical position.
     */
    void set_pointer_position(double x, double y) noexcept;

    /**
     * @brief Add pointer movement delta.
     *
     * @param dx Horizontal movement.
     * @param dy Vertical movement.
     */
    void add_pointer_delta(double dx, double dy) noexcept;

    /**
     * @brief Add wheel movement delta.
     *
     * @param dx Horizontal wheel delta.
     * @param dy Vertical wheel delta.
     */
    void add_wheel_delta(double dx, double dy) noexcept;

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
     * @brief Remove an action binding.
     *
     * @param name Action name.
     * @return true if the action existed and was removed.
     */
    [[nodiscard]] GameBoolResult unbind(
        const std::string &name);

    /**
     * @brief Return true if a key is currently held down.
     *
     * @param key Input key.
     * @return true if key is down.
     */
    [[nodiscard]] bool key_down(InputKey key) const;

    /**
     * @brief Return true if a key was pressed during this frame.
     *
     * @param key Input key.
     * @return true if key transitioned from up to down.
     */
    [[nodiscard]] bool key_pressed(InputKey key) const;

    /**
     * @brief Return true if a key was released during this frame.
     *
     * @param key Input key.
     * @return true if key transitioned from down to up.
     */
    [[nodiscard]] bool key_released(InputKey key) const;

    /**
     * @brief Return true if a pointer button is currently held down.
     *
     * @param button Input button.
     * @return true if button is down.
     */
    [[nodiscard]] bool button_down(InputButton button) const;

    /**
     * @brief Return true if a pointer button was pressed during this frame.
     *
     * @param button Input button.
     * @return true if button transitioned from up to down.
     */
    [[nodiscard]] bool button_pressed(InputButton button) const;

    /**
     * @brief Return true if a pointer button was released during this frame.
     *
     * @param button Input button.
     * @return true if button transitioned from down to up.
     */
    [[nodiscard]] bool button_released(InputButton button) const;

    /**
     * @brief Return true if an action is currently held down.
     *
     * @param name Action name.
     * @return true if action binding is down.
     */
    [[nodiscard]] bool action_down(const std::string &name) const;

    /**
     * @brief Return true if an action was pressed during this frame.
     *
     * @param name Action name.
     * @return true if action binding transitioned from up to down.
     */
    [[nodiscard]] bool action_pressed(const std::string &name) const;

    /**
     * @brief Return true if an action was released during this frame.
     *
     * @param name Action name.
     * @return true if action binding transitioned from down to up.
     */
    [[nodiscard]] bool action_released(const std::string &name) const;

    /**
     * @brief Return the current pointer position.
     */
    [[nodiscard]] PointerPosition pointer_position() const noexcept;

    /**
     * @brief Return the pointer delta accumulated during this frame.
     */
    [[nodiscard]] PointerDelta pointer_delta() const noexcept;

    /**
     * @brief Return the wheel delta accumulated during this frame.
     */
    [[nodiscard]] PointerDelta wheel_delta() const noexcept;

    /**
     * @brief Return the input state.
     */
    [[nodiscard]] InputState &state() noexcept;

    /**
     * @brief Return the input state.
     */
    [[nodiscard]] const InputState &state() const noexcept;

    /**
     * @brief Return the input map.
     */
    [[nodiscard]] InputMap &map() noexcept;

    /**
     * @brief Return the input map.
     */
    [[nodiscard]] const InputMap &map() const noexcept;

    /**
     * @brief Clear all input state and action bindings.
     */
    void clear();

    /**
     * @brief Clear only current and previous input state.
     */
    void clear_state();

    /**
     * @brief Clear only action bindings.
     */
    void clear_bindings();

  private:
    /**
     * @brief Current and previous input state.
     */
    InputState state_{};

    /**
     * @brief Named action bindings.
     */
    InputMap map_{};
  };

} // namespace vix::game

#endif // VIX_GAME_INPUT_SYSTEM_HPP
