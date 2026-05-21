/**
 *
 *  @file WindowBackend.hpp
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
#ifndef VIX_GAME_WINDOW_BACKEND_HPP
#define VIX_GAME_WINDOW_BACKEND_HPP

#include <string>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>

namespace vix::game
{
  /**
   * @brief Backend-independent window implementation interface.
   *
   * WindowBackend is implemented by concrete platform backends:
   * - NullWindowBackend
   * - SDLWindowBackend
   * - GLFWWindowBackend
   * - native platform backends later
   *
   * The public Window class owns a WindowBackend and forwards operations to it.
   */
  class WindowBackend
  {
  public:
    /**
     * @brief Destroy the backend.
     */
    virtual ~WindowBackend() = default;

    /**
     * @brief Open/create the native window.
     *
     * @param config Window configuration.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] virtual GameBoolResult open(
        const WindowConfig &config) = 0;

    /**
     * @brief Close/destroy the native window.
     */
    virtual void close() noexcept = 0;

    /**
     * @brief Poll backend events.
     *
     * This should return all currently available events without blocking.
     *
     * @return Window event list.
     */
    [[nodiscard]] virtual std::vector<WindowEvent> poll_events() = 0;

    /**
     * @brief Present the current frame if the backend supports presentation.
     */
    virtual void present() = 0;

    /**
     * @brief Return true if the window is open.
     */
    [[nodiscard]] virtual bool open() const noexcept = 0;

    /**
     * @brief Return true if the backend received a close request.
     */
    [[nodiscard]] virtual bool close_requested() const noexcept = 0;

    /**
     * @brief Return the current window title.
     */
    [[nodiscard]] virtual const std::string &title() const noexcept = 0;

    /**
     * @brief Set the window title.
     *
     * @param value New title.
     */
    virtual void set_title(std::string value) = 0;

    /**
     * @brief Return the current window width.
     */
    [[nodiscard]] virtual std::uint32_t width() const noexcept = 0;

    /**
     * @brief Return the current window height.
     */
    [[nodiscard]] virtual std::uint32_t height() const noexcept = 0;

    /**
     * @brief Resize the window.
     *
     * @param width New width.
     * @param height New height.
     */
    virtual void resize(
        std::uint32_t width,
        std::uint32_t height) = 0;

    /**
     * @brief Return the backend name.
     */
    [[nodiscard]] virtual const char *backend_name() const noexcept = 0;

    /**
     * @brief Return the native backend handle.
     *
     * Null backends may return nullptr.
     */
    [[nodiscard]] virtual void *native_handle() noexcept = 0;

    /**
     * @brief Return the native backend handle.
     *
     * Null backends may return nullptr.
     */
    [[nodiscard]] virtual const void *native_handle() const noexcept = 0;
  };

} // namespace vix::game

#endif // VIX_GAME_WINDOW_BACKEND_HPP
