/**
 *
 *  @file Scene.hpp
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
#ifndef VIX_GAME_SCENE_HPP
#define VIX_GAME_SCENE_HPP

#include <string>
#include <utility>

#include <vix/game/Frame.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/SceneId.hpp>

namespace vix::game
{
  class App;
  class EventBus;
  class AssetManager;
  class JobSystem;

  class GameContext;
  class InputSystem;
  class Window;
  class Renderer;
  class Renderer2D;

  /**
   * @brief Base class for game scenes.
   *
   * Scene represents a logical state of a game or real-time application.
   * Examples:
   * - main menu
   * - gameplay
   * - pause menu
   * - loading screen
   *
   * V1 keeps Scene intentionally simple:
   * - lifecycle hooks
   * - frame update hook
   * - fixed update hook
   * - access to basic runtime systems through App
   */
  class Scene
  {
  public:
    /**
     * @brief Construct an unnamed scene.
     */
    Scene() = default;

    /**
     * @brief Construct a scene with a name.
     *
     * @param name Scene name.
     */
    explicit Scene(std::string name)
        : name_(std::move(name))
    {
    }

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    /**
     * @brief Destroy the scene.
     */
    virtual ~Scene() = default;

    /**
     * @brief Called when the scene is attached to an app.
     *
     * This happens before on_load().
     *
     * @param app Owning app.
     */
    virtual void on_attach(App &app)
    {
      app_ = &app;
    }

    /**
     * @brief Called when the scene is detached from an app.
     *
     * This happens after on_unload().
     */
    virtual void on_detach()
    {
      app_ = nullptr;
    }

    /**
     * @brief Called when the scene is loaded.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] virtual GameBoolResult on_load()
    {
      loaded_ = true;
      return true;
    }

    /**
     * @brief Called when the scene becomes the active scene.
     */
    virtual void on_enter()
    {
      active_ = true;
    }

    /**
     * @brief Called once per variable frame update.
     *
     * @param frame Current frame data.
     */
    virtual void on_update(const Frame &frame)
    {
      (void)frame;
    }

    /**
     * @brief Called for each fixed simulation step.
     *
     * @param frame Current frame data.
     */
    virtual void on_fixed_update(const Frame &frame)
    {
      (void)frame;
    }

    /**
     * @brief Called when the scene is no longer the active scene.
     */
    virtual void on_exit()
    {
      active_ = false;
    }

    /**
     * @brief Called when the scene is unloaded.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] virtual GameBoolResult on_unload()
    {
      loaded_ = false;
      return true;
    }

    /**
     * @brief Set the scene id.
     *
     * Usually called by SceneManager.
     *
     * @param id Scene id.
     */
    void set_id(SceneId id) noexcept
    {
      id_ = id;
    }

    /**
     * @brief Return the scene id.
     */
    [[nodiscard]] SceneId id() const noexcept
    {
      return id_;
    }

    /**
     * @brief Set the scene name.
     *
     * @param value Scene name.
     * @return Reference to this scene.
     */
    Scene &set_name(std::string value)
    {
      name_ = std::move(value);
      return *this;
    }

    /**
     * @brief Return the scene name.
     */
    [[nodiscard]] const std::string &name() const noexcept
    {
      return name_;
    }

    /**
     * @brief Return true if the scene is attached to an app.
     */
    [[nodiscard]] bool attached() const noexcept
    {
      return app_ != nullptr;
    }

    /**
     * @brief Return true if the scene has been loaded.
     */
    [[nodiscard]] bool loaded() const noexcept
    {
      return loaded_;
    }

    /**
     * @brief Return true if the scene is currently active.
     */
    [[nodiscard]] bool active() const noexcept
    {
      return active_;
    }

    /**
     * @brief Return the owning app.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] App &app()
    {
      return *app_;
    }

    /**
     * @brief Return the owning app.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] const App &app() const
    {
      return *app_;
    }

  protected:
    /**
     * @brief Return the event bus from the owning app.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] EventBus &events();

    /**
     * @brief Return the asset manager from the owning app.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] AssetManager &assets();

    /**
     * @brief Return the job system from the owning app.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] JobSystem &jobs();

    /**
     * @brief Return the game context from the owning app runtime.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] GameContext &context();

    /**
     * @brief Return the input system from the game context.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] InputSystem &input();

    /**
     * @brief Return the window facade from the game context.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] Window &window();

    /**
     * @brief Return the renderer facade from the game context.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] Renderer &renderer();

    /**
     * @brief Return the 2D renderer facade from the game context.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] Renderer2D &renderer2d();

  private:
    /**
     * @brief Scene identifier assigned by SceneManager.
     */
    SceneId id_{invalid_scene_id};

    /**
     * @brief Human-readable scene name.
     */
    std::string name_{};

    /**
     * @brief Owning app.
     */
    App *app_{nullptr};

    /**
     * @brief Whether the scene is loaded.
     */
    bool loaded_{false};

    /**
     * @brief Whether the scene is currently active.
     */
    bool active_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_SCENE_HPP
