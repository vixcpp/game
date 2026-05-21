/**
 *
 *  @file NullWindow.hpp
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
#ifndef VIX_GAME_NULL_WINDOW_HPP
#define VIX_GAME_NULL_WINDOW_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/game/WindowBackend.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>

namespace vix::game
{
  /**
   * @brief Headless/null window backend.
   *
   * NullWindow is a backend implementation that does not create a native
   * platform window.
   *
   * It is useful for:
   * - tests
   * - CI
   * - headless servers
   * - simulations
   * - examples that should not depend on SDL, GLFW, X11, Wayland, Win32, etc.
   */
  class NullWindow final : public WindowBackend
  {
  public:
    /**
     * @brief Construct a closed null window.
     */
    NullWindow() = default;

    NullWindow(const NullWindow &) = delete;
    NullWindow &operator=(const NullWindow &) = delete;

    /**
     * @brief Destroy the null window.
     */
    ~NullWindow() override = default;

    /**
     * @brief Open the null window.
     *
     * @param config Window configuration.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult open(
        const WindowConfig &config) override;

    /**
     * @brief Close the null window.
     */
    void close() noexcept override;

    /**
     * @brief Poll pending null window events.
     *
     * @return Window event list.
     */
    [[nodiscard]] std::vector<WindowEvent> poll_events() override;

    /**
     * @brief Present the current frame.
     *
     * This is a no-op for NullWindow.
     */
    void present() override;

    /**
     * @brief Return true if the null window is open.
     */
    [[nodiscard]] bool open() const noexcept override;

    /**
     * @brief Return true if close was requested.
     */
    [[nodiscard]] bool close_requested() const noexcept override;

    /**
     * @brief Return the current window title.
     */
    [[nodiscard]] const std::string &title() const noexcept override;

    /**
     * @brief Set the window title.
     *
     * @param value New title.
     */
    void set_title(std::string value) override;

    /**
     * @brief Return the current window width.
     */
    [[nodiscard]] std::uint32_t width() const noexcept override;

    /**
     * @brief Return the current window height.
     */
    [[nodiscard]] std::uint32_t height() const noexcept override;

    /**
     * @brief Resize the null window.
     *
     * @param width New width.
     * @param height New height.
     */
    void resize(
        std::uint32_t width,
        std::uint32_t height) override;

    /**
     * @brief Return the backend name.
     */
    [[nodiscard]] const char *backend_name() const noexcept override;

    /**
     * @brief Return the native backend handle.
     *
     * NullWindow always returns nullptr.
     */
    [[nodiscard]] void *native_handle() noexcept override;

    /**
     * @brief Return the native backend handle.
     *
     * NullWindow always returns nullptr.
     */
    [[nodiscard]] const void *native_handle() const noexcept override;

    /**
     * @brief Push an event manually.
     *
     * This is useful for tests and headless simulations.
     *
     * @param event Event to enqueue.
     */
    void push_event(WindowEvent event);

    /**
     * @brief Request closing the window.
     *
     * A close_requested event is also queued.
     */
    void request_close();

  private:
    /**
     * @brief Whether the null window is open.
     */
    bool open_{false};

    /**
     * @brief Whether a close request was received.
     */
    bool close_requested_{false};

    /**
     * @brief Window title.
     */
    std::string title_{"Vix Game"};

    /**
     * @brief Window width.
     */
    std::uint32_t width_{1280};

    /**
     * @brief Window height.
     */
    std::uint32_t height_{720};

    /**
     * @brief Queued events.
     */
    std::vector<WindowEvent> events_{};
  };

} // namespace vix::game

#endif // VIX_GAME_NULL_WINDOW_HPP
