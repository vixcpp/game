/**
 *
 *  @file Renderer.hpp
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
#ifndef VIX_GAME_RENDERER_HPP
#define VIX_GAME_RENDERER_HPP

#include <cstdint>
#include <memory>

#include <vix/game/Color.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/RenderCommand.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/Window.hpp>

namespace vix::game
{
  /**
   * @brief Public backend-independent renderer facade.
   *
   * Renderer owns a RendererBackend and forwards rendering operations to it.
   *
   * The renderer is intentionally separate from Window. A Window owns platform
   * state, while Renderer owns rendering state.
   */
  class Renderer
  {
  public:
    /**
     * @brief Construct an empty renderer without backend.
     */
    Renderer() = default;

    /**
     * @brief Construct a renderer with a backend.
     *
     * @param backend Renderer backend implementation.
     */
    explicit Renderer(std::unique_ptr<RendererBackend> backend) noexcept;

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    Renderer(Renderer &&) noexcept = default;
    Renderer &operator=(Renderer &&) noexcept = default;

    /**
     * @brief Destroy the renderer.
     */
    ~Renderer();

    /**
     * @brief Set the renderer backend.
     *
     * If a previous backend exists, it is shut down first.
     *
     * @param backend New backend.
     * @return Reference to this renderer.
     */
    Renderer &set_backend(std::unique_ptr<RendererBackend> backend) noexcept;

    /**
     * @brief Return true if this renderer has a backend.
     */
    [[nodiscard]] bool has_backend() const noexcept;

    /**
     * @brief Initialize the renderer for a window.
     *
     * @param window Target window.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init(Window &window);

    /**
     * @brief Shut down the renderer.
     */
    void shutdown() noexcept;

    /**
     * @brief Begin rendering a frame.
     */
    void begin_frame();

    /**
     * @brief End rendering a frame.
     */
    void end_frame();

    /**
     * @brief Clear the current render target.
     *
     * @param color Clear color.
     */
    void clear(Color color = Color::black());

    /**
     * @brief Resize the renderer viewport or swapchain.
     *
     * @param width New width.
     * @param height New height.
     */
    void resize(
        std::uint32_t width,
        std::uint32_t height);

    /**
     * @brief Execute one render command.
     *
     * @param command Render command.
     */
    void execute(const RenderCommand &command);

    /**
     * @brief Return true if the renderer backend is initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Return the current render width.
     */
    [[nodiscard]] std::uint32_t width() const noexcept;

    /**
     * @brief Return the current render height.
     */
    [[nodiscard]] std::uint32_t height() const noexcept;

    /**
     * @brief Return the renderer backend name.
     */
    [[nodiscard]] const char *backend_name() const noexcept;

    /**
     * @brief Return a native renderer handle if available.
     */
    [[nodiscard]] void *native_handle() noexcept;

    /**
     * @brief Return a native renderer handle if available.
     */
    [[nodiscard]] const void *native_handle() const noexcept;

    /**
     * @brief Return the backend.
     */
    [[nodiscard]] RendererBackend *backend() noexcept;

    /**
     * @brief Return the backend.
     */
    [[nodiscard]] const RendererBackend *backend() const noexcept;

  private:
    /**
     * @brief Owned renderer backend.
     */
    std::unique_ptr<RendererBackend> backend_{};
  };

} // namespace vix::game

#endif // VIX_GAME_RENDERER_HPP
