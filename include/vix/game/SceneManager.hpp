/**
 *
 *  @file SceneManager.hpp
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
#ifndef VIX_GAME_SCENE_MANAGER_HPP
#define VIX_GAME_SCENE_MANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneData.hpp>
#include <vix/game/SceneId.hpp>

namespace vix::game
{
  class App;

  /**
   * @brief Owns, loads, activates, updates, and unloads scenes.
   *
   * SceneManager is responsible for the scene lifecycle:
   * - register scenes
   * - attach scenes to the app
   * - load and unload scenes
   * - switch the active scene
   * - forward update and fixed update calls
   *
   * V1 supports one active scene at a time.
   */
  class SceneManager
  {
  public:
    /**
     * @brief Construct a scene manager without an app.
     */
    SceneManager() = default;

    /**
     * @brief Construct a scene manager attached to an app.
     *
     * @param app Owning app.
     */
    explicit SceneManager(App &app) noexcept
        : app_(&app)
    {
    }

    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    /**
     * @brief Destroy the scene manager.
     */
    ~SceneManager() = default;

    /**
     * @brief Attach the manager to an app.
     *
     * Already registered scenes are also attached.
     *
     * @param app Owning app.
     */
    void attach(App &app);

    /**
     * @brief Detach all scenes from the app.
     */
    void detach();

    /**
     * @brief Register an existing scene instance.
     *
     * @param name Scene name.
     * @param scene Scene instance.
     * @return Assigned scene id, or a structured error.
     */
    [[nodiscard]] GameResult<SceneId> add(
        std::string name,
        std::unique_ptr<Scene> scene);

    /**
     * @brief Create and register a scene.
     *
     * @tparam T Scene type.
     * @tparam Args Constructor argument types.
     * @param name Scene name.
     * @param args Constructor arguments.
     * @return Assigned scene id, or a structured error.
     */
    template <typename T, typename... Args>
    [[nodiscard]] GameResult<SceneId> create(
        std::string name,
        Args &&...args)
    {
      auto scene = std::make_unique<T>(std::forward<Args>(args)...);
      return add(std::move(name), std::move(scene));
    }

    /**
     * @brief Remove a scene by id.
     *
     * If the scene is active, it is exited before removal.
     *
     * @param id Scene id.
     * @return true if the scene was removed.
     */
    [[nodiscard]] GameBoolResult remove(SceneId id);

    /**
     * @brief Remove a scene by name.
     *
     * @param name Scene name.
     * @return true if the scene was removed.
     */
    [[nodiscard]] GameBoolResult remove(const std::string &name);

    /**
     * @brief Load a scene by id.
     *
     * @param id Scene id.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult load(SceneId id);

    /**
     * @brief Load a scene by name.
     *
     * @param name Scene name.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult load(const std::string &name);

    /**
     * @brief Unload a scene by id.
     *
     * @param id Scene id.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult unload(SceneId id);

    /**
     * @brief Unload a scene by name.
     *
     * @param name Scene name.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult unload(const std::string &name);

    /**
     * @brief Make a scene active by id.
     *
     * The previous active scene receives on_exit().
     * The new active scene is loaded if needed, then receives on_enter().
     *
     * @param id Scene id.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult set_active(SceneId id);

    /**
     * @brief Make a scene active by name.
     *
     * @param name Scene name.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult set_active(const std::string &name);

    /**
     * @brief Update the active scene.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame);

    /**
     * @brief Fixed update the active scene.
     *
     * @param frame Current frame.
     */
    void fixed_update(const Frame &frame);

    /**
     * @brief Return true if a scene id exists.
     */
    [[nodiscard]] bool contains(SceneId id) const;

    /**
     * @brief Return true if a scene name exists.
     */
    [[nodiscard]] bool contains(const std::string &name) const;

    /**
     * @brief Find a scene id by name.
     *
     * @param name Scene name.
     * @return Scene id, or invalid_scene_id if missing.
     */
    [[nodiscard]] SceneId id_for(const std::string &name) const;

    /**
     * @brief Return a scene by id.
     *
     * @param id Scene id.
     * @return Scene pointer, or nullptr if missing.
     */
    [[nodiscard]] Scene *get(SceneId id);

    /**
     * @brief Return a scene by id.
     *
     * @param id Scene id.
     * @return Scene pointer, or nullptr if missing.
     */
    [[nodiscard]] const Scene *get(SceneId id) const;

    /**
     * @brief Return a scene by name.
     *
     * @param name Scene name.
     * @return Scene pointer, or nullptr if missing.
     */
    [[nodiscard]] Scene *get(const std::string &name);

    /**
     * @brief Return a scene by name.
     *
     * @param name Scene name.
     * @return Scene pointer, or nullptr if missing.
     */
    [[nodiscard]] const Scene *get(const std::string &name) const;

    /**
     * @brief Return the active scene.
     */
    [[nodiscard]] Scene *active();

    /**
     * @brief Return the active scene.
     */
    [[nodiscard]] const Scene *active() const;

    /**
     * @brief Return the active scene id.
     */
    [[nodiscard]] SceneId active_id() const noexcept;

    /**
     * @brief Return the active scene name.
     */
    [[nodiscard]] std::string active_name() const;

    /**
     * @brief Return registered scene count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Return true if no scene is registered.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return all registered scene names.
     */
    [[nodiscard]] std::vector<std::string> names() const;

    /**
     * @brief Return a serializable snapshot of the scene manager.
     *
     * @return Scene file data containing registered scenes and active scene.
     */
    [[nodiscard]] SceneFileData to_data() const;

    /**
     * @brief Unload and remove all scenes.
     */
    void clear();

  private:
    /**
     * @brief Internal scene record.
     */
    struct SceneRecord
    {
      SceneId id{invalid_scene_id};
      std::string name{};
      std::unique_ptr<Scene> scene{};
    };

    /**
     * @brief Return a scene record by id.
     */
    [[nodiscard]] SceneRecord *record(SceneId id);

    /**
     * @brief Return a scene record by id.
     */
    [[nodiscard]] const SceneRecord *record(SceneId id) const;

    /**
     * @brief Dispatch a scene event if an event bus is available.
     */
    void dispatch_scene_event(
        EventType type,
        const SceneRecord &record);

    /**
     * @brief Return the next unique scene id.
     */
    [[nodiscard]] SceneId next_id() noexcept;

  private:
    /**
     * @brief Owning app.
     */
    App *app_{nullptr};

    /**
     * @brief Scene records by id.
     */
    std::unordered_map<SceneId, SceneRecord> scenes_{};

    /**
     * @brief Scene name to id index.
     */
    std::unordered_map<std::string, SceneId> name_to_id_{};

    /**
     * @brief Active scene id.
     */
    SceneId active_id_{invalid_scene_id};

    /**
     * @brief Next scene id counter.
     */
    SceneId next_id_{1};
  };

} // namespace vix::game

#endif // VIX_GAME_SCENE_MANAGER_HPP
