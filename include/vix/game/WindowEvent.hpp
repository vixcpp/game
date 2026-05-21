/**
 *
 *  @file WindowEvent.hpp
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
#ifndef VIX_GAME_WINDOW_EVENT_HPP
#define VIX_GAME_WINDOW_EVENT_HPP

#include <cstdint>

#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>

namespace vix::game
{
  /**
   * @brief Backend-independent window event type.
   */
  enum class WindowEventType : std::uint32_t
  {
    /**
     * @brief Unknown event.
     */
    Unknown = 0,

    /**
     * @brief Window close requested.
     */
    CloseRequested,

    /**
     * @brief Window resized.
     */
    Resized,

    /**
     * @brief Window gained focus.
     */
    FocusGained,

    /**
     * @brief Window lost focus.
     */
    FocusLost,

    /**
     * @brief Keyboard key pressed.
     */
    KeyPressed,

    /**
     * @brief Keyboard key released.
     */
    KeyReleased,

    /**
     * @brief Pointer button pressed.
     */
    ButtonPressed,

    /**
     * @brief Pointer button released.
     */
    ButtonReleased,

    /**
     * @brief Pointer moved.
     */
    PointerMoved,

    /**
     * @brief Mouse wheel moved.
     */
    WheelMoved
  };

  /**
   * @brief Convert a window event type to a stable readable name.
   *
   * @param type Window event type.
   * @return Static event type name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      WindowEventType type) noexcept
  {
    switch (type)
    {
    case WindowEventType::Unknown:
      return "unknown";

    case WindowEventType::CloseRequested:
      return "close_requested";

    case WindowEventType::Resized:
      return "resized";

    case WindowEventType::FocusGained:
      return "focus_gained";

    case WindowEventType::FocusLost:
      return "focus_lost";

    case WindowEventType::KeyPressed:
      return "key_pressed";

    case WindowEventType::KeyReleased:
      return "key_released";

    case WindowEventType::ButtonPressed:
      return "button_pressed";

    case WindowEventType::ButtonReleased:
      return "button_released";

    case WindowEventType::PointerMoved:
      return "pointer_moved";

    case WindowEventType::WheelMoved:
      return "wheel_moved";
    }

    return "unknown";
  }

  /**
   * @brief Backend-independent window event.
   *
   * WindowEvent is a small value object produced by Window backends and consumed
   * by higher-level systems such as InputSystem or App.
   */
  struct WindowEvent
  {
    /**
     * @brief Event type.
     */
    WindowEventType type{WindowEventType::Unknown};

    /**
     * @brief Window width for resize events.
     */
    std::uint32_t width{0};

    /**
     * @brief Window height for resize events.
     */
    std::uint32_t height{0};

    /**
     * @brief Keyboard key for key events.
     */
    InputKey key{InputKey::Unknown};

    /**
     * @brief Pointer button for button events.
     */
    InputButton button{InputButton::Unknown};

    /**
     * @brief Pointer x position.
     */
    double x{0.0};

    /**
     * @brief Pointer y position.
     */
    double y{0.0};

    /**
     * @brief Delta x value.
     *
     * Used for pointer move or wheel events.
     */
    double dx{0.0};

    /**
     * @brief Delta y value.
     *
     * Used for pointer move or wheel events.
     */
    double dy{0.0};

    /**
     * @brief Create a close-requested event.
     */
    [[nodiscard]] static constexpr WindowEvent close_requested() noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::CloseRequested;
      return event;
    }

    /**
     * @brief Create a resize event.
     *
     * @param w New width.
     * @param h New height.
     */
    [[nodiscard]] static constexpr WindowEvent resized(
        std::uint32_t w,
        std::uint32_t h) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::Resized;
      event.width = w;
      event.height = h;
      return event;
    }

    /**
     * @brief Create a key pressed event.
     *
     * @param value Input key.
     */
    [[nodiscard]] static constexpr WindowEvent key_pressed(
        InputKey value) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::KeyPressed;
      event.key = value;
      return event;
    }

    /**
     * @brief Create a key released event.
     *
     * @param value Input key.
     */
    [[nodiscard]] static constexpr WindowEvent key_released(
        InputKey value) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::KeyReleased;
      event.key = value;
      return event;
    }

    /**
     * @brief Create a pointer button pressed event.
     *
     * @param value Input button.
     */
    [[nodiscard]] static constexpr WindowEvent button_pressed(
        InputButton value) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::ButtonPressed;
      event.button = value;
      return event;
    }

    /**
     * @brief Create a pointer button released event.
     *
     * @param value Input button.
     */
    [[nodiscard]] static constexpr WindowEvent button_released(
        InputButton value) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::ButtonReleased;
      event.button = value;
      return event;
    }

    /**
     * @brief Create a pointer moved event.
     *
     * @param px Pointer x position.
     * @param py Pointer y position.
     * @param delta_x Pointer x delta.
     * @param delta_y Pointer y delta.
     */
    [[nodiscard]] static constexpr WindowEvent pointer_moved(
        double px,
        double py,
        double delta_x,
        double delta_y) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::PointerMoved;
      event.x = px;
      event.y = py;
      event.dx = delta_x;
      event.dy = delta_y;
      return event;
    }

    /**
     * @brief Create a wheel moved event.
     *
     * @param delta_x Wheel x delta.
     * @param delta_y Wheel y delta.
     */
    [[nodiscard]] static constexpr WindowEvent wheel_moved(
        double delta_x,
        double delta_y) noexcept
    {
      WindowEvent event;
      event.type = WindowEventType::WheelMoved;
      event.dx = delta_x;
      event.dy = delta_y;
      return event;
    }

    /**
     * @brief Return true if this event requests window closing.
     */
    [[nodiscard]] constexpr bool is_close_request() const noexcept
    {
      return type == WindowEventType::CloseRequested;
    }

    /**
     * @brief Return true if this event is a keyboard event.
     */
    [[nodiscard]] constexpr bool is_key_event() const noexcept
    {
      return type == WindowEventType::KeyPressed ||
             type == WindowEventType::KeyReleased;
    }

    /**
     * @brief Return true if this event is a pointer button event.
     */
    [[nodiscard]] constexpr bool is_button_event() const noexcept
    {
      return type == WindowEventType::ButtonPressed ||
             type == WindowEventType::ButtonReleased;
    }

    /**
     * @brief Return true if this event is a pointer motion event.
     */
    [[nodiscard]] constexpr bool is_pointer_event() const noexcept
    {
      return type == WindowEventType::PointerMoved ||
             is_button_event();
    }
  };

} // namespace vix::game

#endif // VIX_GAME_WINDOW_EVENT_HPP
