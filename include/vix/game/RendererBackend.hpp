/**
 *
 *  @file RendererBackend.hpp
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
#ifndef VIX_GAME_RENDERER_BACKEND_HPP
#define VIX_GAME_RENDERER_BACKEND_HPP

#include <cstdint>

#include <vix/game/Color.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/Window.hpp>

namespace vix::game
{
  /**
   * @brief Backend-independent renderer implementation interface.
   *
   * RendererBackend is implemented by concrete render backends:
   * - NullRenderer
   * - SoftwareRenderer
   * - OpenGLRenderer
   * - VulkanRenderer
   * - MetalRenderer
   * - DirectXRenderer
   *
   * The public Renderer class owns a RendererBackend and forwards rendering
   * operations to it.
   */
  class RendererBackend
  {
  public:
    /**
     * @brief Destroy the renderer backend.
     */
    virtual ~RendererBackend() = default;

    /**
     * @brief Initialize the renderer backend for a window.
     *
     * @param window Target window.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] virtual GameBoolResult init(Window &window) = 0;

    /**
     * @brief Shut down the renderer backend.
     */
    virtual void shutdown() noexcept = 0;

    /**
     * @brief Begin rendering a frame.
     */
    virtual void begin_frame() = 0;

    /**
     * @brief End rendering a frame.
     */
    virtual void end_frame() = 0;

    /**
     * @brief Clear the current render target.
     *
     * @param color Clear color.
     */
    virtual void clear(Color color) = 0;

    /**
     * @brief Resize the renderer viewport or swapchain.
     *
     * @param width New width.
     * @param height New height.
     */
    virtual void resize(
        std::uint32_t width,
        std::uint32_t height) = 0;

    /**
     * @brief Return true if the backend has been initialized.
     */
    [[nodiscard]] virtual bool initialized() const noexcept = 0;

    /**
     * @brief Return the current render width.
     */
    [[nodiscard]] virtual std::uint32_t width() const noexcept = 0;

    /**
     * @brief Return the current render height.
     */
    [[nodiscard]] virtual std::uint32_t height() const noexcept = 0;

    /**
     * @brief Return the renderer backend name.
     */
    [[nodiscard]] virtual const char *backend_name() const noexcept = 0;

    /**
     * @brief Return a native backend handle if available.
     *
     * Null backends may return nullptr.
     */
    [[nodiscard]] virtual void *native_handle() noexcept = 0;

    /**
     * @brief Return a native backend handle if available.
     *
     * Null backends may return nullptr.
     */
    [[nodiscard]] virtual const void *native_handle() const noexcept = 0;
  };

} // namespace vix::game

#endif // VIX_GAME_RENDERER_BACKEND_HPP
