/**
 *
 *  @file NullRenderer.hpp
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
#ifndef VIX_GAME_NULL_RENDERER_HPP
#define VIX_GAME_NULL_RENDERER_HPP

#include <cstdint>

#include <vix/game/Color.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/Window.hpp>

namespace vix::game
{
  /**
   * @brief Headless/null renderer backend.
   *
   * NullRenderer implements RendererBackend without performing real rendering.
   *
   * It is useful for:
   * - tests
   * - CI
   * - headless mode
   * - server-side simulations
   * - validating renderer-facing code without graphics dependencies
   */
  class NullRenderer final : public RendererBackend
  {
  public:
    /**
     * @brief Construct a null renderer.
     */
    NullRenderer() = default;

    NullRenderer(const NullRenderer &) = delete;
    NullRenderer &operator=(const NullRenderer &) = delete;

    /**
     * @brief Destroy the null renderer.
     */
    ~NullRenderer() override = default;

    /**
     * @brief Initialize the null renderer for a window.
     *
     * @param window Target window.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init(Window &window) override;

    /**
     * @brief Shut down the null renderer.
     */
    void shutdown() noexcept override;

    /**
     * @brief Begin a render frame.
     */
    void begin_frame() override;

    /**
     * @brief End a render frame.
     */
    void end_frame() override;

    /**
     * @brief Clear the virtual render target.
     *
     * @param color Clear color.
     */
    void clear(Color color) override;

    /**
     * @brief Resize the virtual render target.
     *
     * @param width New width.
     * @param height New height.
     */
    void resize(
        std::uint32_t width,
        std::uint32_t height) override;

    /**
     * @brief Return true if the renderer has been initialized.
     */
    [[nodiscard]] bool initialized() const noexcept override;

    /**
     * @brief Return the current render width.
     */
    [[nodiscard]] std::uint32_t width() const noexcept override;

    /**
     * @brief Return the current render height.
     */
    [[nodiscard]] std::uint32_t height() const noexcept override;

    /**
     * @brief Return the renderer backend name.
     */
    [[nodiscard]] const char *backend_name() const noexcept override;

    /**
     * @brief Return the native renderer handle.
     *
     * NullRenderer always returns nullptr.
     */
    [[nodiscard]] void *native_handle() noexcept override;

    /**
     * @brief Return the native renderer handle.
     *
     * NullRenderer always returns nullptr.
     */
    [[nodiscard]] const void *native_handle() const noexcept override;

    /**
     * @brief Return true if a frame is currently open.
     */
    [[nodiscard]] bool frame_active() const noexcept;

    /**
     * @brief Return the last clear color.
     */
    [[nodiscard]] Color clear_color() const noexcept;

  private:
    /**
     * @brief Whether the null renderer is initialized.
     */
    bool initialized_{false};

    /**
     * @brief Whether begin_frame() was called without a matching end_frame().
     */
    bool frame_active_{false};

    /**
     * @brief Virtual render width.
     */
    std::uint32_t width_{0};

    /**
     * @brief Virtual render height.
     */
    std::uint32_t height_{0};

    /**
     * @brief Last clear color.
     */
    Color clear_color_{Color::black()};
  };

} // namespace vix::game

#endif // VIX_GAME_NULL_RENDERER_HPP
