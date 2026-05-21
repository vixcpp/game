/**
 *
 *  @file App.hpp
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
#ifndef VIX_GAME_APP_HPP
#define VIX_GAME_APP_HPP

#include <memory>
#include <string>
#include <utility>

#include <vix/game/AppConfig.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameLoop.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/AsyncAssetLoader.hpp>

namespace vix::game
{
  class AssetManager;
  class EventBus;
  class JobSystem;
  class SceneManager;

  /**
   * @brief Main application facade for vix/game.
   *
   * App coordinates the core systems of a real-time application:
   * - configuration
   * - game loop
   * - scenes
   * - events
   * - assets
   * - background jobs
   *
   * App does not provide rendering, windowing, audio, physics, or input in V1.
   * It provides the foundation needed to build those systems on top.
   */
  class App
  {
  public:
    /**
     * @brief Construct an app with default configuration.
     */
    App();

    /**
     * @brief Construct an app with explicit configuration.
     *
     * @param config Application configuration.
     */
    explicit App(AppConfig config);

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    /**
     * @brief Destroy the app.
     */
    ~App();

    /**
     * @brief Initialize app systems.
     *
     * This prepares the app before running the loop.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init();

    /**
     * @brief Run the application.
     *
     * If init() was not called manually, run() initializes the app first.
     *
     * @return true on normal completion, or a structured error.
     */
    [[nodiscard]] GameBoolResult run();

    /**
     * @brief Request the application to stop.
     */
    void stop() noexcept;

    /**
     * @brief Shut down app systems.
     *
     * This function is idempotent.
     */
    void shutdown() noexcept;

    /**
     * @brief Return true if the app has been initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Return true if the app is currently running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Return the app configuration.
     */
    [[nodiscard]] const AppConfig &config() const noexcept;

    /**
     * @brief Replace the app configuration.
     *
     * This should be done before init() or run().
     *
     * @param config New configuration.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult set_config(AppConfig config);

    /**
     * @brief Return the game loop.
     */
    [[nodiscard]] GameLoop &loop() noexcept;

    /**
     * @brief Return the game loop.
     */
    [[nodiscard]] const GameLoop &loop() const noexcept;

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
     * @brief Return the job system.
     */
    [[nodiscard]] JobSystem &jobs();

    /**
     * @brief Return the job system.
     */
    [[nodiscard]] const JobSystem &jobs() const;

    /**
     * @brief Return the async asset loader.
     */
    [[nodiscard]] AsyncAssetLoader &async_assets();

    /**
     * @brief Return the async asset loader.
     */
    [[nodiscard]] const AsyncAssetLoader &async_assets() const;

    /**
     * @brief Set the per-frame update callback.
     *
     * This callback is forwarded to the internal game loop.
     *
     * @tparam Fn Callable type compatible with void(const Frame&).
     * @param fn Callback to invoke every frame.
     */
    template <typename Fn>
    void on_update(Fn &&fn)
    {
      loop_.set_update_callback(std::forward<Fn>(fn));
    }

    /**
     * @brief Set the fixed update callback.
     *
     * This callback is forwarded to the internal game loop.
     *
     * @tparam Fn Callable type compatible with void(const Frame&).
     * @param fn Callback to invoke for fixed simulation steps.
     */
    template <typename Fn>
    void on_fixed_update(Fn &&fn)
    {
      loop_.set_fixed_update_callback(std::forward<Fn>(fn));
    }

    /**
     * @brief Set the app title.
     *
     * @param title New app title.
     * @return Reference to this app.
     */
    App &set_title(std::string title);

    /**
     * @brief Set target FPS.
     *
     * @param fps Target frames per second.
     * @return Reference to this app.
     */
    App &set_target_fps(std::uint32_t fps) noexcept;

  private:
    /**
     * @brief Create owned subsystems.
     */
    void create_systems();

    /**
     * @brief Configure the game loop callbacks used internally by App.
     */
    void configure_loop();

    /**
     * @brief Per-frame app update.
     *
     * This is invoked by GameLoop and forwards updates to scenes and systems.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame);

    /**
     * @brief Fixed app update.
     *
     * @param frame Current frame.
     */
    void fixed_update(const Frame &frame);

  private:
    /**
     * @brief Application configuration.
     */
    AppConfig config_{};

    /**
     * @brief Main game loop.
     */
    GameLoop loop_{};

    /**
     * @brief Scene manager.
     */
    std::unique_ptr<SceneManager> scenes_{};

    /**
     * @brief Event bus.
     */
    std::unique_ptr<EventBus> events_{};

    /**
     * @brief Asset manager.
     */
    std::unique_ptr<AssetManager> assets_{};

    /**
     * @brief Background job system.
     */
    std::unique_ptr<JobSystem> jobs_{};

    /**
     * @brief Async asset loader.
     */
    std::unique_ptr<AsyncAssetLoader> async_assets_{};

    /**
     * @brief Whether init() completed successfully.
     */
    bool initialized_{false};

    /**
     * @brief Whether shutdown() has already been executed.
     */
    bool shutdown_done_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_APP_HPP
