/**
 *
 *  @file InputState.hpp
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
#ifndef VIX_GAME_INPUT_STATE_HPP
#define VIX_GAME_INPUT_STATE_HPP

#include <cstdint>
#include <unordered_set>

#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>

namespace vix::game
{
  /**
   * @brief 2D pointer position.
   */
  struct PointerPosition
  {
    /**
     * @brief Horizontal position.
     */
    double x{0.0};

    /**
     * @brief Vertical position.
     */
    double y{0.0};
  };

  /**
   * @brief 2D pointer delta.
   */
  struct PointerDelta
  {
    /**
     * @brief Horizontal movement.
     */
    double x{0.0};

    /**
     * @brief Vertical movement.
     */
    double y{0.0};
  };

  /**
   * @brief Current and previous input state.
   *
   * InputState stores backend-independent keyboard, mouse button, pointer,
   * and wheel state.
   *
   * Backends update this object with press/release/move/wheel events.
   * Game code then queries pressed, released, held, and pointer state.
   */
  class InputState
  {
  public:
    /**
     * @brief Construct an empty input state.
     */
    InputState() = default;

    /**
     * @brief Begin a new input frame.
     *
     * This snapshots the current state as the previous state and clears
     * per-frame pointer deltas and wheel deltas.
     */
    void begin_frame();

    /**
     * @brief Mark a key as currently pressed.
     *
     * @param key Input key.
     */
    void press_key(InputKey key);

    /**
     * @brief Mark a key as currently released.
     *
     * @param key Input key.
     */
    void release_key(InputKey key);

    /**
     * @brief Mark a pointer button as currently pressed.
     *
     * @param button Input button.
     */
    void press_button(InputButton button);

    /**
     * @brief Mark a pointer button as currently released.
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
     * @brief Add mouse wheel delta.
     *
     * @param dx Horizontal wheel delta.
     * @param dy Vertical wheel delta.
     */
    void add_wheel_delta(double dx, double dy) noexcept;

    /**
     * @brief Return true if a key is currently held down.
     *
     * @param key Input key.
     * @return true if currently down.
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
     * @brief Return true if a button is currently held down.
     *
     * @param button Input button.
     * @return true if currently down.
     */
    [[nodiscard]] bool button_down(InputButton button) const;

    /**
     * @brief Return true if a button was pressed during this frame.
     *
     * @param button Input button.
     * @return true if button transitioned from up to down.
     */
    [[nodiscard]] bool button_pressed(InputButton button) const;

    /**
     * @brief Return true if a button was released during this frame.
     *
     * @param button Input button.
     * @return true if button transitioned from down to up.
     */
    [[nodiscard]] bool button_released(InputButton button) const;

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
     * @brief Return true if any key or button is currently held down.
     */
    [[nodiscard]] bool any_down() const noexcept;

    /**
     * @brief Clear all current and previous input state.
     */
    void clear();

  private:
    /**
     * @brief Currently pressed keys.
     */
    std::unordered_set<InputKey> current_keys_{};

    /**
     * @brief Previously pressed keys.
     */
    std::unordered_set<InputKey> previous_keys_{};

    /**
     * @brief Currently pressed pointer buttons.
     */
    std::unordered_set<InputButton> current_buttons_{};

    /**
     * @brief Previously pressed pointer buttons.
     */
    std::unordered_set<InputButton> previous_buttons_{};

    /**
     * @brief Current pointer position.
     */
    PointerPosition pointer_{};

    /**
     * @brief Pointer movement accumulated during the current frame.
     */
    PointerDelta pointer_delta_{};

    /**
     * @brief Wheel movement accumulated during the current frame.
     */
    PointerDelta wheel_delta_{};
  };

} // namespace vix::game

#endif // VIX_GAME_INPUT_STATE_HPP
