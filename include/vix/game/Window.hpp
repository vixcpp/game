/**
 *
 *  @file Window.hpp
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
#ifndef VIX_GAME_WINDOW_HPP
#define VIX_GAME_WINDOW_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/game/WindowBackend.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>

namespace vix::game
{
  /**
   * @brief Public backend-independent window facade.
   *
   * Window owns a WindowBackend and forwards window operations to it.
   *
   * V2 keeps Window independent from rendering. A renderer can later use
   * native_handle() when it needs access to the backend-specific handle.
   */
  class Window
  {
  public:
    /**
     * @brief Construct an empty window without backend.
     */
    Window() = default;

    /**
     * @brief Construct a window with a backend.
     *
     * @param backend Window backend implementation.
     */
    explicit Window(std::unique_ptr<WindowBackend> backend) noexcept;

    /**
     * @brief Construct and open a window with a backend and config.
     *
     * @param backend Window backend implementation.
     * @param config Window configuration.
     */
    Window(
        std::unique_ptr<WindowBackend> backend,
        WindowConfig config);

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&) noexcept = default;
    Window &operator=(Window &&) noexcept = default;

    /**
     * @brief Destroy the window.
     */
    ~Window();

    /**
     * @brief Set the backend.
     *
     * If a previous backend exists, it is closed first.
     *
     * @param backend New backend.
     * @return Reference to this window.
     */
    Window &set_backend(std::unique_ptr<WindowBackend> backend) noexcept;

    /**
     * @brief Return true if this window has a backend.
     */
    [[nodiscard]] bool has_backend() const noexcept;

    /**
     * @brief Open the window using the configured backend.
     *
     * @param config Window configuration.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult open(const WindowConfig &config);

    /**
     * @brief Close the window.
     */
    void close() noexcept;

    /**
     * @brief Poll pending window events.
     *
     * @return Window event list.
     */
    [[nodiscard]] std::vector<WindowEvent> poll_events();

    /**
     * @brief Present the current frame.
     */
    void present();

    /**
     * @brief Return true if the window is open.
     */
    [[nodiscard]] bool open() const noexcept;

    /**
     * @brief Return true if the window received a close request.
     */
    [[nodiscard]] bool close_requested() const noexcept;

    /**
     * @brief Return the current window title.
     */
    [[nodiscard]] const std::string &title() const noexcept;

    /**
     * @brief Set the window title.
     *
     * @param value New title.
     */
    void set_title(std::string value);

    /**
     * @brief Return the current window width.
     */
    [[nodiscard]] std::uint32_t width() const noexcept;

    /**
     * @brief Return the current window height.
     */
    [[nodiscard]] std::uint32_t height() const noexcept;

    /**
     * @brief Resize the window.
     *
     * @param width New width.
     * @param height New height.
     */
    void resize(
        std::uint32_t width,
        std::uint32_t height);

    /**
     * @brief Return the backend name.
     */
    [[nodiscard]] const char *backend_name() const noexcept;

    /**
     * @brief Return the native backend handle.
     */
    [[nodiscard]] void *native_handle() noexcept;

    /**
     * @brief Return the native backend handle.
     */
    [[nodiscard]] const void *native_handle() const noexcept;

    /**
     * @brief Return the backend.
     */
    [[nodiscard]] WindowBackend *backend() noexcept;

    /**
     * @brief Return the backend.
     */
    [[nodiscard]] const WindowBackend *backend() const noexcept;

  private:
    /**
     * @brief Empty title returned when no backend exists.
     */
    static const std::string &empty_title() noexcept;

  private:
    /**
     * @brief Owned backend implementation.
     */
    std::unique_ptr<WindowBackend> backend_{};
  };

} // namespace vix::game

#endif // VIX_GAME_WINDOW_HPP
