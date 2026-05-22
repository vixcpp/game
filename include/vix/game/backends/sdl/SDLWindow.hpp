/**
 *
 *  @file SDLWindow.hpp
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
#ifndef VIX_GAME_BACKENDS_SDL_SDL_WINDOW_HPP
#define VIX_GAME_BACKENDS_SDL_SDL_WINDOW_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include <vix/game/GameResult.hpp>
#include <vix/game/WindowBackend.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>

namespace vix::game::sdl
{
  /**
   * @brief SDL2 window backend for vix/game.
   *
   * SDLWindow implements the backend-independent WindowBackend interface using
   * SDL2.
   *
   * It is responsible for:
   * - creating and destroying a native SDL window
   * - polling SDL events
   * - converting SDL events to WindowEvent
   * - exposing the native SDL_Window handle
   */
  class SDLWindow final : public WindowBackend
  {
  public:
    /**
     * @brief Construct a closed SDL window backend.
     */
    SDLWindow() = default;

    SDLWindow(const SDLWindow &) = delete;
    SDLWindow &operator=(const SDLWindow &) = delete;

    /**
     * @brief Destroy the SDL window backend.
     */
    ~SDLWindow() override;

    /**
     * @brief Open/create the SDL window.
     *
     * @param config Window configuration.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult open(
        const WindowConfig &config) override;

    /**
     * @brief Close/destroy the SDL window.
     */
    void close() noexcept override;

    /**
     * @brief Poll pending SDL events.
     *
     * @return Backend-independent window event list.
     */
    [[nodiscard]] std::vector<WindowEvent> poll_events() override;

    /**
     * @brief Present the current frame.
     *
     * For SDLWindow alone this is a no-op. A renderer backend owns real
     * presentation.
     */
    void present() override;

    /**
     * @brief Return true if the SDL window is open.
     */
    [[nodiscard]] bool open() const noexcept override;

    /**
     * @brief Return true if SDL received a close request.
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
     * @brief Resize the SDL window.
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
     * @brief Return the native SDL_Window handle.
     */
    [[nodiscard]] void *native_handle() noexcept override;

    /**
     * @brief Return the native SDL_Window handle.
     */
    [[nodiscard]] const void *native_handle() const noexcept override;

  private:
    /**
     * @brief Build SDL window flags from WindowConfig.
     */
    [[nodiscard]] static std::uint32_t make_window_flags(
        const WindowConfig &config) noexcept;

  private:
    /**
     * @brief Native SDL window.
     */
    SDL_Window *window_{nullptr};

    /**
     * @brief Whether this backend initialized SDL video.
     */
    bool sdl_video_initialized_{false};

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
  };

} // namespace vix::game::sdl

#endif // VIX_GAME_BACKENDS_SDL_SDL_WINDOW_HPP
