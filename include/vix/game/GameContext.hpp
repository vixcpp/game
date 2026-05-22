/**
 *
 *  @file GameContext.hpp
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
#ifndef VIX_GAME_GAME_CONTEXT_HPP
#define VIX_GAME_GAME_CONTEXT_HPP

#include <memory>

#include <vix/game/AsyncAssetLoader.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/InputSystem.hpp>
#include <vix/game/JobSystem.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/SceneManager.hpp>
#include <vix/game/Window.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/WindowBackend.hpp>

namespace vix::game
{
  class App;

  /**
   * @brief Runtime access point for game systems.
   *
   * GameContext provides a stable way to access the main game systems without
   * passing many individual references around.
   *
   * It is designed for:
   * - editor runtime
   * - scripting integration
   * - tools
   * - scene utilities
   * - future audio and physics systems
   */
  class GameContext
  {
  public:
    /**
     * @brief Construct an empty context.
     */
    GameContext() = default;

    /**
     * @brief Construct a context attached to an app.
     *
     * @param app Runtime app.
     */
    explicit GameContext(App &app) noexcept;

    GameContext(const GameContext &) = delete;
    GameContext &operator=(const GameContext &) = delete;

    /**
     * @brief Destroy the context.
     */
    ~GameContext() = default;

    /**
     * @brief Attach the context to an app.
     *
     * @param app Runtime app.
     * @return Reference to this context.
     */
    GameContext &attach(App &app) noexcept;

    /**
     * @brief Detach the context from its app.
     */
    void detach() noexcept;

    /**
     * @brief Return true if the context is attached to an app.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Return the app.
     */
    [[nodiscard]] App &app();

    /**
     * @brief Return the app.
     */
    [[nodiscard]] const App &app() const;

    /**
     * @brief Return the scene manager.
     */
    [[nodiscard]] SceneManager &scenes();

    /**
     * @brief Return the scene manager.
     */
    [[nodiscard]] const SceneManager &scenes() const;

    /**
     * @brief Return the event bus.
     */
    [[nodiscard]] EventBus &events();

    /**
     * @brief Return the event bus.
     */
    [[nodiscard]] const EventBus &events() const;

    /**
     * @brief Return the asset manager.
     */
    [[nodiscard]] AssetManager &assets();

    /**
     * @brief Return the asset manager.
     */
    [[nodiscard]] const AssetManager &assets() const;

    /**
     * @brief Return the async asset loader.
     */
    [[nodiscard]] AsyncAssetLoader &async_assets();

    /**
     * @brief Return the async asset loader.
     */
    [[nodiscard]] const AsyncAssetLoader &async_assets() const;

    /**
     * @brief Return the job system.
     */
    [[nodiscard]] JobSystem &jobs();

    /**
     * @brief Return the job system.
     */
    [[nodiscard]] const JobSystem &jobs() const;

    /**
     * @brief Return the input system.
     */
    [[nodiscard]] InputSystem &input() noexcept;

    /**
     * @brief Return the input system.
     */
    [[nodiscard]] const InputSystem &input() const noexcept;

    /**
     * @brief Return the window facade.
     */
    [[nodiscard]] Window &window() noexcept;

    /**
     * @brief Return the window facade.
     */
    [[nodiscard]] const Window &window() const noexcept;

    /**
     * @brief Return the renderer facade.
     */
    [[nodiscard]] Renderer &renderer() noexcept;

    /**
     * @brief Return the renderer facade.
     */
    [[nodiscard]] const Renderer &renderer() const noexcept;

    /**
     * @brief Return the 2D renderer facade.
     */
    [[nodiscard]] Renderer2D &renderer2d() noexcept;

    /**
     * @brief Return the 2D renderer facade.
     */
    [[nodiscard]] const Renderer2D &renderer2d() const noexcept;

    /**
     * @brief Set the window backend used by the runtime window facade.
     *
     * @param backend Window backend implementation.
     * @return Reference to this context.
     */
    GameContext &set_window_backend(
        std::unique_ptr<WindowBackend> backend) noexcept;

    /**
     * @brief Set the renderer backend used by the runtime renderer facade.
     *
     * @param backend Renderer backend implementation.
     * @return Reference to this context.
     */
    GameContext &set_renderer_backend(
        std::unique_ptr<RendererBackend> backend) noexcept;

    /**
     * @brief Begin a runtime frame.
     *
     * This prepares per-frame systems such as input.
     */
    void begin_frame();

    /**
     * @brief End a runtime frame.
     */
    void end_frame();

    /**
     * @brief Clear runtime-owned transient state.
     */
    void clear();

  private:
    /**
     * @brief Attached app.
     */
    App *app_{nullptr};

    /**
     * @brief Runtime input system.
     */
    InputSystem input_{};

    /**
     * @brief Runtime window facade.
     */
    Window window_{};

    /**
     * @brief Runtime renderer facade.
     */
    Renderer renderer_{};

    /**
     * @brief Runtime 2D renderer facade.
     */
    Renderer2D renderer2d_{};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_CONTEXT_HPP
