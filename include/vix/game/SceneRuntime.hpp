/**
 *
 *  @file SceneRuntime.hpp
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
#ifndef VIX_GAME_SCENE_RUNTIME_HPP
#define VIX_GAME_SCENE_RUNTIME_HPP

#include <memory>
#include <string>

#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneId.hpp>
#include <vix/game/SceneSerializer.hpp>

namespace vix::game
{
  /**
   * @brief Runtime helper for scene operations.
   *
   * SceneRuntime provides a higher-level API around SceneManager.
   *
   * It is designed for:
   * - editor runtime
   * - scripting integration
   * - scene tools
   * - scene loading/saving workflows
   */
  class SceneRuntime
  {
  public:
    /**
     * @brief Construct an empty scene runtime.
     */
    SceneRuntime() = default;

    /**
     * @brief Construct a scene runtime attached to a game context.
     *
     * @param context Game context.
     */
    explicit SceneRuntime(GameContext &context) noexcept;

    SceneRuntime(const SceneRuntime &) = delete;
    SceneRuntime &operator=(const SceneRuntime &) = delete;

    /**
     * @brief Destroy the scene runtime.
     */
    ~SceneRuntime() = default;

    /**
     * @brief Attach to a game context.
     *
     * @param context Game context.
     * @return Reference to this runtime.
     */
    SceneRuntime &attach(GameContext &context) noexcept;

    /**
     * @brief Detach from the game context.
     */
    void detach() noexcept;

    /**
     * @brief Return true if attached to a game context.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] GameContext &context();

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] const GameContext &context() const;

    /**
     * @brief Add a scene instance.
     *
     * @param name Scene name.
     * @param scene Scene instance.
     * @return Scene id, or a structured error.
     */
    [[nodiscard]] GameResult<SceneId> add(
        std::string name,
        std::unique_ptr<Scene> scene);

    /**
     * @brief Create a scene.
     *
     * @tparam T Scene type.
     * @tparam Args Constructor argument types.
     * @param name Scene name.
     * @param args Constructor arguments.
     * @return Scene id, or a structured error.
     */
    template <typename T, typename... Args>
    [[nodiscard]] GameResult<SceneId> create(
        std::string name,
        Args &&...args)
    {
      return context().scenes().create<T>(
          std::move(name),
          std::forward<Args>(args)...);
    }

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
     * @brief Activate a scene by id.
     *
     * @param id Scene id.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult activate(SceneId id);

    /**
     * @brief Activate a scene by name.
     *
     * @param name Scene name.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult activate(const std::string &name);

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
     * @brief Remove a scene by id.
     *
     * @param id Scene id.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult remove(SceneId id);

    /**
     * @brief Remove a scene by name.
     *
     * @param name Scene name.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult remove(const std::string &name);

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
     * @brief Return a serializable snapshot of the scene manager.
     *
     * This is useful for editor tools, diagnostics, and scene inspection.
     */
    [[nodiscard]] SceneFileData inspect() const;

    /**
     * @brief Save scene manager state to a file.
     *
     * @param path Destination file path.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult save_file(const std::string &path) const;

    /**
     * @brief Load scene manager state from a file.
     *
     * @param path Source file path.
     * @param factory Scene factory.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult load_file(
        const std::string &path,
        SceneSerializer::SceneFactory factory);

    /**
     * @brief Clear all scenes.
     */
    void clear();

  private:
    /**
     * @brief Attached game context.
     */
    GameContext *context_{nullptr};
  };

} // namespace vix::game

#endif // VIX_GAME_SCENE_RUNTIME_HPP
