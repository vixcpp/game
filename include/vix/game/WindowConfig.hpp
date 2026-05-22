/**
 *
 *  @file WindowConfig.hpp
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
#ifndef VIX_GAME_WINDOW_CONFIG_HPP
#define VIX_GAME_WINDOW_CONFIG_HPP

#include <cstdint>
#include <string>
#include <utility>

#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Configuration used to create a game window.
   *
   * WindowConfig is backend-independent. Platform-specific backends can map
   * this configuration to SDL, GLFW, native Win32, X11, Wayland, Cocoa, or a
   * null/headless window.
   */
  struct WindowConfig
  {
    /**
     * @brief Window title.
     */
    std::string title{"Vix Game"};

    /**
     * @brief Initial window width in pixels.
     */
    std::uint32_t width{1280};

    /**
     * @brief Initial window height in pixels.
     */
    std::uint32_t height{720};

    /**
     * @brief Whether the window starts fullscreen.
     */
    bool fullscreen{false};

    /**
     * @brief Whether the window can be resized.
     */
    bool resizable{true};

    /**
     * @brief Whether the window starts visible.
     */
    bool visible{true};

    /**
     * @brief Whether vertical sync is requested.
     */
    bool vsync{true};

    /**
     * @brief Whether the backend should create a headless/null window.
     */
    bool headless{false};

    /**
     * @brief Whether the window should be created with OpenGL support.
     */
    bool opengl{false};

    /**
     * @brief Create default window config.
     */
    [[nodiscard]] static WindowConfig defaults() noexcept;

    /**
     * @brief Create a headless window config.
     */
    [[nodiscard]] static WindowConfig headless_config() noexcept;

    /**
     * @brief Validate the window configuration.
     *
     * @return true if valid, otherwise a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const;

    /**
     * @brief Set the window title.
     *
     * @param value New title.
     * @return Reference to this config.
     */
    WindowConfig &set_title(std::string value);

    /**
     * @brief Set the window size.
     *
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @return Reference to this config.
     */
    WindowConfig &set_size(
        std::uint32_t w,
        std::uint32_t h) noexcept;

    /**
     * @brief Return true if the configured size is valid.
     */
    [[nodiscard]] bool valid_size() const noexcept;
  };

} // namespace vix::game

#endif // VIX_GAME_WINDOW_CONFIG_HPP
